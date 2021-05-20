#include "MiniMath/Ast/Evaluator.hpp"
#include "MiniMath/Ast/Printer.hpp"
#include "MiniMath/Ast/Errors.hpp"
#include "MiniMath/Ast/EnvironmentReducer.hpp"
#include "MiniMath/IoUtility.hpp"
#include "MiniMath/Lexer.hpp"
#include "MiniMath/MiniMathParser.hpp"

#include <fmt/format.h>

#include <fstream>
#include <functional>

namespace mm::ast
{
    using namespace expr;
    using namespace ops;

    struct BinaryExprEvaluator : StrictVisitor
    {
        Expr operator()(Add, NumberExpr lhs, NumberExpr rhs) const
        {
            return lhs + rhs;
        }

        Expr operator()(Subtract, NumberExpr lhs, NumberExpr rhs) const
        {
            return lhs - rhs;
        }

        Expr operator()(Multiply, NumberExpr lhs, NumberExpr rhs) const
        {
            return lhs * rhs;
        }

        Expr operator()(Divide, NumberExpr lhs, NumberExpr rhs) const
        {
            return lhs / rhs;
        }

        Expr operator()(And, BoolExpr lhs, BoolExpr rhs) const
        {
            return lhs && rhs;
        }

        Expr operator()(Or, BoolExpr lhs, BoolExpr rhs) const
        {
            return lhs || rhs;
        }

        Expr operator()(Less, NumberExpr lhs, NumberExpr rhs) const
        {
            return lhs < rhs;
        }

        Expr operator()(LessThan, NumberExpr lhs, NumberExpr rhs) const
        {
            return lhs <= rhs;
        }

        Expr operator()(Greater, NumberExpr lhs, NumberExpr rhs) const
        {
            return lhs > rhs;
        }

        Expr operator()(GreaterThan, NumberExpr lhs, NumberExpr rhs) const
        {
            return lhs >= rhs;
        }

        Expr operator()(Equals, NumberExpr lhs, NumberExpr rhs) const
        {
            return lhs == rhs;
        }

        Expr operator()(Equals, BoolExpr lhs, BoolExpr rhs) const
        {
            return lhs == rhs;
        }

        // using auto directly causes an ICE
        template <typename A, typename B, typename C>
        Expr operator()(A const& op, B const& lhs, C const& rhs) const
        {
            throw TypeError(fmt::format("Invalid operation: {} {} {}.", lhs, op, rhs));
        }
    };

    Expr ExprEvaluator::operator()(BinaryExpr const& expr, Environment const& env) const
    {
        auto lhs = visit(*this, expr.left, env);
        auto rhs = visit(*this, expr.right, env);

        return visitMany(BinaryExprEvaluator{}, expr.operation, lhs, rhs);
    }

    Expr ExprEvaluator::operator()(CallExpr const& expr, Environment const& env) const
    {
        auto target = visit(*this, expr.target, env);
        auto closure = tryGetExpr<Closure>(target);

        if (!closure)
            throw TypeError(fmt::format("'{}' is not a function", target));

        if (closure->paramNames.size() != expr.args.size())
            throw ArgumentError(fmt::format("'{}' expects {} arguments, but {} were given", target,
                closure->paramNames.size(), expr.args.size()));

        auto callEnv = closure->environment;

        for (std::size_t i = 0; i < expr.args.size(); i++)
        {
            auto&& argName = closure->paramNames[i];
            auto argVal = visit(*this, expr.args[i], env);
            callEnv.add(argName, argVal);
        }

        return visit(*this, closure->body, callEnv);
    }

    Expr ExprEvaluator::operator()(NameExpr const& expr, Environment const& env) const
    {
        if (auto val = env.tryGet(expr.name))
            return *val;

        throw LookupError(fmt::format("Unknown identifier '{}'", expr.name));
    }

    Expr ExprEvaluator::operator()(NumberExpr expr, Environment const& env) const
    {
        return expr;
    }

    Expr ExprEvaluator::operator()(BoolExpr expr, Environment const& env) const
    {
        return expr;
    }

    Expr ExprEvaluator::operator()(Closure const& expr, Environment const& env) const
    {
        return makeExpr(expr);
    }

    Expr ExprEvaluator::operator()(LetExpr const& expr, Environment const& env) const
    {
        auto letEnv = env.with(expr.name, visit(*this, expr.value, env));
        return visit(*this, expr.body, letEnv);
    }

    Expr ExprEvaluator::operator()(FunctionExpr const& expr, Environment const& env) const
    {
        Environment closureEnv;
        EnvironmentReducer{}(expr, {}, env, closureEnv);

        return makeExpr(Closure{
            .environment = std::move(closureEnv),
            .paramNames = expr.paramNames,
            .body = expr.body
        });
    }

    Expr ExprEvaluator::operator()(expr::InverseExpr const& expr, Environment const& env) const
    {
        auto body = visit(*this, expr.body, env);

        if (auto val = tryGetExpr<NumberExpr>(body))
            return -*val;

        throw TypeError(fmt::format("Expected number. {} is not invertible", body));
    }

    Expr ExprEvaluator::operator()(IfExpr const& expr, Environment const& env) const
    {
        auto result = visit(*this, expr.condition, env);

        if (auto cond = tryGetExpr<BoolExpr>(result))
            return *cond
                ? visit(*this, expr.thenArm, env)
                : visit(*this, expr.elseArm, env);

        throw TypeError(fmt::format("if condition expected a boolean, but got {}", result));
    }

    Environment StmtEvaluator::operator()(stmt::LetStmt const& stmt, Environment const& env) const
    {
        auto val = evaluate(stmt.value, env);
        return Environment{}.with(stmt.name, std::move(val));
    }

    Environment StmtEvaluator::operator()(stmt::ImportStmt const& stmt, Environment const& env) const
    {
        auto target = stmt.target;
        std::ranges::replace(target, '.', '/');

        auto path = env.getDir() / target;
        path.replace_extension(".mm");

        if (!exists(path))
            throw LookupError(fmt::format("Module '{}' doesn't exist.", stmt.target));

        return evaluate(path);
    }

    Environment evaluate(std::filesystem::path const& codeFile)
    {
        std::ifstream file(codeFile);
        skipBom(file);

        Environment env;
        env.setDir(codeFile.parent_path());
        evaluate(env, [&]
        {
            auto c = file.get();
            return file ? char(c) : 0;
        });

        return env;
    }

    void evaluate(Environment& env, std::function<char()> charSource)
    {
        Lexer lexer(move(charSource));

        MiniMathParser parser([&] { return lexer.nextToken(); });

        while (!lexer.atEof())
        {
            auto s = parser.parseStatement();
            env.merge(evaluate(s, env));
        }
    }
}

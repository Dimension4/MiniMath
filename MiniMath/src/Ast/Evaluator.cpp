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

    struct BinaryExprEvaluator
    {
        Expr operator()(Add, NumberExpr const& lhs, NumberExpr const& rhs) const
        {
            return NumberExpr{ lhs.value + rhs.value };
        }

        Expr operator()(Subtract, NumberExpr const& lhs, NumberExpr const& rhs) const
        {
            return NumberExpr{ lhs.value - rhs.value };
        }

        Expr operator()(Multiply, NumberExpr const& lhs, NumberExpr const& rhs) const
        {
            return NumberExpr{ lhs.value * rhs.value };
        }

        Expr operator()(Divide, NumberExpr const& lhs, NumberExpr const& rhs) const
        {
            return NumberExpr{ lhs.value / rhs.value };
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

    Expr ExprEvaluator::operator()(NumberExpr const& expr, Environment const& env) const
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

        std::ifstream file(path);
        skipBom(file);

        Lexer lexer([&]
        {
            auto c = file.get();
            return file ? char(c) : 0;
        });

        MiniMathParser parser(std::bind_front(&Lexer::nextToken, std::ref(lexer)));

        Environment importEnv;
        importEnv.setDir(path.parent_path());

        while (!lexer.atEof())
        {
            auto s = parser.parseStatement();
            importEnv.merge(evaluate(s, importEnv));
        }

        return importEnv;
    }
}

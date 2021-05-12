#include "MiniMath/Ast/Evaluator.hpp"
#include "MiniMath/Ast/Printer.hpp"

#include <fmt/format.h>

#include <cmath>

namespace mm::ast
{
    using namespace expr;

    Expr ExprEvaluator::operator()(BinaryExpr const& expr, Environment const& env) const
    {
        auto lhs = tryGetExpr<ConstantExpr>(visit(*this, expr.left, env));

        if (!lhs)
            throw TypeError("Left side of binary expression is not a number");

        auto rhs = tryGetExpr<ConstantExpr>(visit(*this, expr.right, env));

        if (!rhs)
            throw TypeError("Right side of binary expression is not a number");

        switch (expr.operation)
        {
        case BinaryOperation::Add: return ConstantExpr(lhs->value + rhs->value);
        case BinaryOperation::Subtract: return ConstantExpr(lhs->value - rhs->value);
        case BinaryOperation::Multiply: return ConstantExpr(lhs->value * rhs->value);
        case BinaryOperation::Divide: return ConstantExpr(lhs->value / rhs->value);
        case BinaryOperation::Power: return ConstantExpr(std::pow(lhs->value, rhs->value));
        }

        throw std::logic_error("unreachable");
    }

    Expr ExprEvaluator::operator()(CallExpr const& expr, Environment const& env) const
    {
        auto target = visit(*this, expr.target, env);
        auto closure = tryGetExpr<Closure>(target);

        if (!closure)
            throw TypeError(fmt::format("'{}' is not a function", target));

        if (closure->paramNames.size() != expr.args.size())
            throw ArgumentError(fmt::format("'{}' expects {} arguments, but {} were given", target, closure->paramNames.size(), expr.args.size()));

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

    Expr ExprEvaluator::operator()(ConstantExpr const& expr, Environment const& env) const
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

    Environment StmtEvaluator::operator()(stmt::LetStmt const& stmt, Environment const& env) const
    {
    	auto val = evaluate(stmt.value, env);
        return Environment{}.with(std::move(stmt.name), std::move(val));
    }
}

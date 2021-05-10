#pragma once

#include "Environment.hpp"
#include "../Expressions.hpp"

#include <stdexcept>

namespace mm::ast
{
    struct Evaluator
    {
        Expr operator()(expr::BinaryExpr const& expr, Environment const& env) const;
        Expr operator()(expr::CallExpr const& expr, Environment const& env) const;
        Expr operator()(expr::NameExpr const& expr, Environment const& env) const;
        Expr operator()(expr::ConstantExpr const& expr, Environment const& env) const;
        Expr operator()(expr::Closure const& expr, Environment const& env) const;
        Expr operator()(expr::LetExpr const& expr, Environment const& env) const;
    };

    inline Expr evaluate(Expr const& expr, Environment const& env = {})
    {
        return visit(Evaluator{}, expr, env);
    }

    struct TypeError : std::logic_error
    {
        using std::logic_error::logic_error;
    };

    struct LookupError : std::logic_error
    {
        using std::logic_error::logic_error;
    };

    struct ArgumentError : std::logic_error
    {
        using std::logic_error::logic_error;
    };
}

#pragma once

#include "Environment.hpp"
#include "../Expressions.hpp"

#include <stdexcept>

namespace mm::ast
{
    struct Evaluator
    {
        Expr operator()(expressions::BinaryExpr const& expr, Environment const& env) const;
        Expr operator()(expressions::CallExpr const& expr, Environment const& env) const;
        Expr operator()(expressions::NameExpr const& expr, Environment const& env) const;
        Expr operator()(expressions::ConstantExpr const& expr, Environment const& env) const;
        Expr operator()(expressions::Closure const& expr, Environment const& env) const;
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

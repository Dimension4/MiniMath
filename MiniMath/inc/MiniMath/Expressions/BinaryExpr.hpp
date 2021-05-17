#pragma once

#include "Expr.hpp"

namespace mm::expr
{
    namespace ops
    {
        struct Add {};

        struct Subtract {};

        struct Multiply {};

        struct Divide {};

        using BinaryOperationBase = std::variant<Add, Subtract, Multiply, Divide>;
    }

    struct BinaryOperation : ops::BinaryOperationBase
    {
        using ops::BinaryOperationBase::BinaryOperationBase;
        using ops::BinaryOperationBase::operator=;

        friend bool operator==(const BinaryOperation& lhs, const BinaryOperation& rhs)
        {
            return lhs.index() == rhs.index();
        }
    };

    struct BinaryExpr
    {
        BinaryOperation operation{};
        Expr left;
        Expr right;

        friend bool operator==(BinaryExpr const&, BinaryExpr const&) = default;
    };
}

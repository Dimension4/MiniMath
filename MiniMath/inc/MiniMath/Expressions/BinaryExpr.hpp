#pragma once

#include "Expr.hpp"
#include "../TypeTraits.hpp"

namespace mm::expr
{
    namespace ops
    {
        struct Add {};

        struct Subtract {};

        struct Multiply {};

        struct Divide {};

        using BinaryOperationBase = std::variant<Add, Subtract, Multiply, Divide>;

        template <VariantMember<BinaryOperationBase> T>
        constexpr bool operator==(T, T)
        {
            return true;
        }
    }

    struct BinaryOperation : ops::BinaryOperationBase
    {
        using ops::BinaryOperationBase::BinaryOperationBase;
        using ops::BinaryOperationBase::operator=;
    };

    struct BinaryExpr
    {
        BinaryOperation operation{};
        Expr left;
        Expr right;

        friend bool operator==(BinaryExpr const&, BinaryExpr const&) = default;
    };
}

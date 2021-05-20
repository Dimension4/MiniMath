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

        struct And {};

        struct Or {};

        struct Less {};

        struct LessThan {};

        struct Greater {};

        struct GreaterThan {};

        struct Equals {};

        using BinaryOperationBase = std::variant<
            Add,
            Subtract,
            Multiply,
            Divide,
            And,
            Or,
            Less,
            LessThan,
            Greater,
            GreaterThan,
            Equals
        >;

        template <VariantMember<BinaryOperationBase> T>
        constexpr bool operator==(T,
            T)
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

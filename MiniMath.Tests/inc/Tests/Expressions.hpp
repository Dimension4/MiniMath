#pragma once

#include "MiniMath/Expressions.hpp"

#include <cstddef>

namespace expr
{
    inline mm::Expr operator""_name(char const* val, std::size_t)
    {
        return mm::expr::NameExpr{ val };
    }

    inline mm::Expr operator""_const(long double val)
    {
        return mm::expr::ConstantExpr{ double(val) };
    }

    inline mm::Expr operator""_const(unsigned long long val)
    {
        return mm::expr::ConstantExpr{ double(val) };
    }

    inline mm::Expr binexpr(mm::expr::BinaryOperation op, mm::Expr left, mm::Expr right)
    {
        return mm::makeExpr(mm::expr::BinaryExpr{
            .operation = op,
            .left = std::move(left),
            .right = std::move(right)
        });
    }
}

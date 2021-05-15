#pragma once

#include "Expr.hpp"

#include <string_view>

namespace mm::expr
{
    enum class BinaryOperation
    {
        Add,
        Subtract,
        Multiply,
        Divide,
        Power
    };

    [[nodiscard]]
    constexpr std::string_view enumName(BinaryOperation op) noexcept
    {
        switch (op)
        {
#define ENUM_CASE(x) case BinaryOperation::x: return #x

            ENUM_CASE(Add);
            ENUM_CASE(Subtract);
            ENUM_CASE(Multiply);
            ENUM_CASE(Divide);
            ENUM_CASE(Power);

#undef ENUM_CASE
        }

        return "<BAD>";
    }

    [[nodiscard]]
    constexpr std::string_view enumDisplay(BinaryOperation op) noexcept
    {
        switch (op)
        {
        case BinaryOperation::Add: return "+";
        case BinaryOperation::Subtract: return "-";
        case BinaryOperation::Multiply: return "*";
        case BinaryOperation::Divide: return "/";
        case BinaryOperation::Power: return "^";
        }

        return "<BAD>";
    }

    struct BinaryExpr
    {
        BinaryOperation operation{};
        Expr left;
        Expr right;

        friend bool operator==(BinaryExpr const&, BinaryExpr const&) = default;
    };
}

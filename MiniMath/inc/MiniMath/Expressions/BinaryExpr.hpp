#pragma once

#include "Expr.hpp"
#include "Recursive.hpp"

#include <string_view>

namespace mm::expressions
{
    enum class BinaryOperation
    {
        Add,
        Subtract,
        Multiply,
        Divide,
        Modulo,
        Power
    };

    constexpr std::string_view enumName(BinaryOperation op) noexcept
    {
        switch (op)
        {
#define ENUM_CASE(x) case BinaryOperation::x: return #x

            ENUM_CASE(Add);
            ENUM_CASE(Subtract);
            ENUM_CASE(Multiply);
            ENUM_CASE(Divide);
            ENUM_CASE(Modulo);
            ENUM_CASE(Power);

#undef ENUM_CASE
        }

        return "<BAD>";
    }

    constexpr std::string_view enumDisplay(BinaryOperation op) noexcept
    {
        switch (op)
        {
        case BinaryOperation::Add: return "+";
        case BinaryOperation::Subtract: return "-";
        case BinaryOperation::Multiply: return "*";
        case BinaryOperation::Divide: return "/";
        case BinaryOperation::Modulo: return "mod";
        case BinaryOperation::Power: return "^";
        }

        return "<BAD>";
    }

    struct BinaryExpr
    {
        BinaryOperation operation{};
        Expr left;
        Expr right;
    };
}

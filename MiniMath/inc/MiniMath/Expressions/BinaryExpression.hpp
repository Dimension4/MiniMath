﻿#pragma once

#include "Expression.hpp"

#include <fmt/format.h>

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

    class BinaryExpression : public Expression
    {
    public:
        BinaryExpression(BinaryOperation operation, ExpressionPtr left, ExpressionPtr right) :
            operation(operation),
            left(move(left)),
            right(move(right)) {}

        BinaryOperation operation{};
        ExpressionPtr left{};
        ExpressionPtr right{};

        [[nodiscard]] std::string toString() const override
        {
            return fmt::format("({} {} {})", left->toString(), enumDisplay(operation), right->toString());
        }
    };
}

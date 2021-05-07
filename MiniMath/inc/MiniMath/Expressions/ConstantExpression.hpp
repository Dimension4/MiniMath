#pragma once

#include "Expression.hpp"

#include <string>

namespace mm::expressions
{
    class ConstantExpression : public Expression
    {
    public:
        double value;

        explicit ConstantExpression(double value) : value(value) {}

        [[nodiscard]] std::string toString() const override
        {
            return std::to_string(value);
        }
    };
}

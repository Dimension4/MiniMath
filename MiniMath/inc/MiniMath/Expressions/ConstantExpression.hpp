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

        std::string toString() override
        {
            return std::to_string(value);
        }
    };
}

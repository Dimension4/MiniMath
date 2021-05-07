#pragma once

#include "Expression.hpp"

#include <string>

namespace mm::expressions
{
    class NameExpression : public Expression
    {
    public:
        std::string name;

        explicit NameExpression(std::string_view name) : name(name) {}

        [[nodiscard]] std::string toString() const override
        {
            return name;
        }
    };
}

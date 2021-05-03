#pragma once

#include <memory>
#include <string>

namespace mm::expressions
{
    class Expression
    {
    public:
        virtual ~Expression() = default;
        virtual std::string toString() = 0;
    };

    using ExpressionPtr = std::unique_ptr<Expression>;
}

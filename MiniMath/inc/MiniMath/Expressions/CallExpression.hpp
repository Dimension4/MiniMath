#pragma once

#include "Expression.hpp"

#include <fmt/format.h>

#include <vector>

namespace mm::expressions
{
    class CallExpression : public Expression
    {
    public:
        CallExpression(ExpressionPtr target, std::vector<ExpressionPtr> args)
            : target_(move(target)), args_(move(args)) {}

        [[nodiscard]] std::string toString() const override
        {
            return fmt::format("{}({})", target_->toString(), fmt::join(args_, ", "));
        }

    private:
        ExpressionPtr target_;
        std::vector<ExpressionPtr> args_;
    };

    using ExpressionPtr = std::unique_ptr<Expression>;
}

#pragma once

#include "../PrattParser.hpp"
#include "../Expressions/ConstantExpression.hpp"

namespace mm::parselets
{
    class ConstantParselet : public PrefixParselet
    {
    public:
        [[nodiscard]] expressions::ExpressionPtr parse(PrattParser& parser, const Token& token) const override
        {
            return std::make_unique<expressions::ConstantExpression>(std::strtod(token.lexeme.data(), nullptr));
        }
    };
}

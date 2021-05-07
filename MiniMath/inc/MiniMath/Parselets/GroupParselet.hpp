#pragma once

#include "../PrattParser.hpp"

namespace mm::parselets
{
    class GroupParselet : public PrefixParselet
    {
    public:
        [[nodiscard]] expressions::ExpressionPtr parse(PrattParser& parser, const Token& token) const override
        {
            auto inner = parser.parseExpression();
            parser.consume(TokenType::RParen);
            return inner;
        }
    };
}

#pragma once

#include "../PrattParser.hpp"
#include "../Expressions/NameExpression.hpp"

namespace mm::parselets
{
    class NameParselet : public PrefixParselet
    {
    public:
        [[nodiscard]] expressions::ExpressionPtr parse(PrattParser& parser, const Token& token) const override
        {
            return std::make_unique<expressions::NameExpression>(token.lexeme);
        }
    };
}

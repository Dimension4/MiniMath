#pragma once

#include "../PrattParser.hpp"
#include "../Expressions/NameExpression.hpp"
#include "../OperatorPrecedences.hpp"
#include "MiniMath/Expressions/CallExpression.hpp"

namespace mm::parselets
{
    class CallParselet : public InfixParselet
    {
    public:
        [[nodiscard]] expressions::ExpressionPtr parse(PrattParser& parser, expressions::ExpressionPtr left, const Token& token) const override
        {
            std::vector<expressions::ExpressionPtr> args;

            if (!parser.match(TokenType::RParen))
            {
                do
                    args.push_back(parser.parseExpression());
                while (parser.match(TokenType::Comma));

                parser.consume(TokenType::RParen);
            }

            return std::make_unique<expressions::CallExpression>(move(left), move(args));
        }

        [[nodiscard]] int getPrecedence() const override
        {
            return int(OperatorPrecedence::Call);
        }
    };
}

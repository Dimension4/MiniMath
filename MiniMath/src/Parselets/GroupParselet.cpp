#include "MiniMath/Parselets/GroupParselet.hpp"

#include "MiniMath/Expressions.hpp"
#include "MiniMath/PrattParser.hpp"

namespace mm::parselets
{
    Expr GroupParselet::parse(PrattParser& parser, const Token& token) const
    {
        auto inner = parser.parseExpression();
        parser.consume(TokenType::RParen);
        return inner;
    }
}

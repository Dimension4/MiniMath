#include "MiniMath/Parselets/IfPraselet.hpp"

#include "MiniMath/Expressions.hpp"
#include "MiniMath/PrattParser.hpp"

namespace mm::parselets
{
    Expr IfParselet::parse(PrattParser& parser, const Token& token) const
    {
        auto condition = parser.parseExpression();
        parser.consume(TokenType::Then);
        auto thenArm = parser.parseExpression();
        parser.consume(TokenType::Else);
        auto elseArm = parser.parseExpression();

        return makeExpr(expr::IfExpr{ std::move(condition), std::move(thenArm), std::move(elseArm) });
    }
}

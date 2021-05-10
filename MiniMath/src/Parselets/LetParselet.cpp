#include "MiniMath/Parselets/LetParselet.hpp"

#include "MiniMath/Expressions.hpp"
#include "MiniMath/PrattParser.hpp"

#include <string>


namespace mm::parselets
{
    using namespace expr;

    Expr LetParselet::parse(PrattParser& parser, const Token& token) const
    {
        auto name = parser.consume(TokenType::Identifier);
        parser.consume(TokenType::Equals);
        auto value = parser.parseExpression();
        parser.consume(TokenType::In);
        auto body = parser.parseExpression();

        return makeExpr(LetExpr{.name = move(name.lexeme), .value = std::move(value), .body = std::move(body)});
    }
}

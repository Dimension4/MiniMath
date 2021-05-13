#include "MiniMath/Parselets/FunctionParselet.hpp"

#include "MiniMath/Expressions.hpp"
#include "MiniMath/PrattParser.hpp"

namespace mm::parselets
{
    using namespace expr;

    Expr FunctionParselet::parse(PrattParser& parser, const Token& token) const
    {
        std::vector<std::string> params;
        while (auto t = parser.match(TokenType::Identifier))
            params.push_back(move(t->lexeme));
        parser.consume(TokenType::RArrow);
        auto body = parser.parseExpression();

        return makeExpr(FunctionExpr{
            .paramNames = move(params),
            .body = move(body)
        });
    }
}

#include "MiniMath/Parselets/CallParselet.hpp"

#include "MiniMath/Expressions.hpp"
#include "MiniMath/PrattParser.hpp"

#include <vector>

namespace mm::parselets
{
    Expr CallParselet::parse(PrattParser& parser, Expr left, const Token& token) const
    {
        std::vector<Expr> args;

        if (!parser.match(TokenType::RParen))
        {
            do
                args.push_back(parser.parseExpression());
            while (parser.match(TokenType::Comma));

            parser.consume(TokenType::RParen);
        }

        return makeExpr<expressions::CallExpr>(std::move(left), std::move(args));
    }
}

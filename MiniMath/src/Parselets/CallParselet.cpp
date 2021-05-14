#include "MiniMath/Parselets/CallParselet.hpp"

#include "MiniMath/Expressions.hpp"
#include "MiniMath/PrattParser.hpp"

#include <vector>

namespace mm::parselets
{
    Expr CallParselet::parse(PrattParser& parser, const Expr& left, const Token& token) const
    {
        std::vector<Expr> args;

        while (!parser.match(TokenType::RParen))
            args.push_back(parser.parseExpression());

        return makeExpr(expr::CallExpr{ .target = left, .args = std::move(args) });
    }
}

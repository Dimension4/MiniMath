#include "MiniMath/Parselets/PrefixOpParselet.hpp"

#include "MiniMath/PrattParser.hpp"
#include "MiniMath/Expressions.hpp"

namespace mm::parselets
{
    using namespace expr;

    PrefixOpParselet::PrefixOpParselet(int precedence) :
        precedence_(precedence) { }

    Expr PrefixOpParselet::parse(PrattParser& parser, const Token& token) const
    {
        return makeExpr(InverseExpr{ parser.parseExpression(precedence_) });
    }
}

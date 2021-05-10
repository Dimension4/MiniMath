#include "MiniMath/Parselets/ConstantParselet.hpp"

#include "MiniMath/Expressions.hpp"

#include <cstdlib>

namespace mm::parselets
{
    Expr ConstantParselet::parse(PrattParser& parser, const Token& token) const
    {
        return makeExpr(expressions::ConstantExpr{ .value = std::strtod(token.lexeme.data(), nullptr) });
    }
}

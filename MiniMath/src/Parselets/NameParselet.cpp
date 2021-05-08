#include "MiniMath/Parselets/NameParselet.hpp"

#include "MiniMath/Expressions.hpp"
#include "MiniMath/PrattParser.hpp"

#include <string>

namespace mm::parselets
{
    Expr NameParselet::parse(PrattParser& parser, const Token& token) const
    {
        return makeExpr<expressions::NameExpr>(std::string(token.lexeme));
    }
}

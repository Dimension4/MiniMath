#include "MiniMath/Parselets/ConstantParselet.hpp"

#include "MiniMath/Expressions.hpp"

#include <cstdlib>

namespace mm::parselets
{
    Expr ConstantParselet::parse(PrattParser& parser, const Token& token) const
    {
        switch (token.type)
        {
        case TokenType::Number:
            return std::strtod(token.lexeme.data(), nullptr);

        case TokenType::True:
            return true;

        case TokenType::False:
            return false;
        }

        throw std::logic_error("Unreachable");
    }
}

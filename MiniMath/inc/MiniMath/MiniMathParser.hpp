#pragma once

#include "PrattParser.hpp"
#include "OperatorPrecedences.hpp"
#include "Parselets/BinaryOpParselet.hpp"

namespace mm
{
    class MiniMathParser : public PrattParser
    {
    public:
        [[nodiscard]]
        explicit MiniMathParser(std::function<Token()> tokenSource);

    private:
        void infix(TokenType token, OperatorPrecedence precedence, parselets::Associativity associativity = parselets::Associativity::Left);
    };
}

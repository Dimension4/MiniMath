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
        explicit MiniMathParser(Lexer& lexer);

    private:
        void prefix(TokenType token, OperatorPrecedence precedence);
        void postfix(TokenType token, OperatorPrecedence precedence);
        void infix(TokenType token, OperatorPrecedence precedence, parselets::Associativity associativity = parselets::Associativity::Left);
    };
}

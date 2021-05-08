#pragma once

#include "../Token.hpp"

namespace mm
{
    class PrattParser;
    class Expr;
}

namespace mm::parselets
{
    class InfixParselet
    {
    public:
        virtual ~InfixParselet() = default;
        [[nodiscard]] virtual Expr parse(PrattParser& parser, Expr left, const Token& token) const = 0;
        [[nodiscard]] virtual int getPrecedence() const = 0;
    };
}

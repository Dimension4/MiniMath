#pragma once

#include "InfixParselet.hpp"

namespace mm::parselets
{
    enum class Associativity
    {
        Left,
        Right
    };

    class BinaryOpParselet : public InfixParselet
    {
    public:
        [[nodiscard]] explicit BinaryOpParselet(int precedence, Associativity associativity);
        [[nodiscard]] Expr parse(PrattParser& parser, const Expr& left, const Token& token) const override;
        [[nodiscard]] int getPrecedence() const override;

    private:
        int precedence_ = 0;
        Associativity associativity_ = Associativity::Left;
    };
}

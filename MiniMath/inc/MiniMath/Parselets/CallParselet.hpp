#pragma once

#include "InfixParselet.hpp"
#include "../OperatorPrecedences.hpp"

namespace mm::parselets
{
    class CallParselet : public InfixParselet
    {
    public:
        [[nodiscard]]
        Expr parse(PrattParser& parser, const Expr& left, const Token& token) const override;

        [[nodiscard]]
        int getPrecedence() const override
        {
            return int(OperatorPrecedence::Call);
        }
    };
}

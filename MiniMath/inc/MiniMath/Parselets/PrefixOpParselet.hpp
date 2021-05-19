#pragma once

#include "PrefixParselet.hpp"
#include "MiniMath/OperatorPrecedences.hpp"

namespace mm::parselets
{
    class PrefixOpParselet : public PrefixParselet
    {
    public:
        [[nodiscard]]
        explicit PrefixOpParselet(int precedence);

        [[nodiscard]]
        Expr parse(PrattParser& parser, const Token& token) const override;

    private:
        int precedence_;
    };
}

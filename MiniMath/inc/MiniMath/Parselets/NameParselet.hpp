#pragma once

#include "PrefixParselet.hpp"

namespace mm::parselets
{
    class NameParselet : public PrefixParselet
    {
    public:
        [[nodiscard]]
        Expr parse(PrattParser& parser, const Token& token) const override;
    };
}

#pragma once

#include "../Token.hpp"

namespace mm
{
    class PrattParser;
    class Expr;
}

namespace mm::parselets
{
    class PrefixParselet
    {
    public:
        virtual ~PrefixParselet() = default;

        [[nodiscard]]
        virtual Expr parse(PrattParser& parser, const Token& token) const = 0;
    };
}

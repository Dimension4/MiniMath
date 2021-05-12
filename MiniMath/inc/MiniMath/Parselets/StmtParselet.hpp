#pragma once

#include "../Token.hpp"

namespace mm
{
    class PrattParser;
    class Stmt;
}

namespace mm::parselets
{
    class StmtParselet
    {
    public:
        virtual ~StmtParselet() = default;

        [[nodiscard]]
        virtual Stmt parse(PrattParser& parser, const Token& token) const = 0;
    };
}

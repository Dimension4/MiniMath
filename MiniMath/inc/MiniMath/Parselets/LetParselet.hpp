#pragma once

#include "PrefixParselet.hpp"
#include "StmtParselet.hpp"

namespace mm::parselets
{
    class LetParselet : public PrefixParselet
    {
    public:
        [[nodiscard]]
        Expr parse(PrattParser& parser, const Token& token) const override;
    };

	
    class LetStmtParselet : public StmtParselet
    {
    public:
        [[nodiscard]]
        Stmt parse(PrattParser& parser, const Token& token) const override;
    };
}

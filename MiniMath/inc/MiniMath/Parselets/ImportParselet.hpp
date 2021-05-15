#pragma once

#include "StmtParselet.hpp"

namespace mm::parselets
{	
    class ImportParselet : public StmtParselet
    {
    public:
        [[nodiscard]]
        Stmt parse(PrattParser& parser, const Token& token) const override;
    };
}

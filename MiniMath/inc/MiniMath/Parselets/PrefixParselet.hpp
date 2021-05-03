#pragma once

#include "../Expressions/Expression.hpp"
#include "../Token.hpp"

namespace mm
{
    class PrattParser;

    namespace parselets
    {
        class PrefixParselet
        {
        public:
            virtual ~PrefixParselet() = default;
            [[nodiscard]] virtual expressions::ExpressionPtr parse(PrattParser& parser, const Token& token) const = 0;
        };
    }
}

#pragma once

#include "../Expressions/Expression.hpp"
#include "../Token.hpp"

namespace mm
{
    class PrattParser;

    namespace parselets
    {
        class InfixParselet
        {
        public:
            virtual ~InfixParselet() = default;
            [[nodiscard]] virtual expressions::ExpressionPtr parse(PrattParser& parser, expressions::ExpressionPtr left, const Token& token) const = 0;
            [[nodiscard]] virtual int getPrecedence() const = 0;
        };
    }
}

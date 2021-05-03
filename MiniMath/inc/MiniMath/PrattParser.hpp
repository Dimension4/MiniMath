#pragma once

#include "Lexer.hpp"
#include "Expressions/Expression.hpp"
#include "Parselets/PrefixParselet.hpp"
#include "Parselets/InfixParselet.hpp"

#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

namespace mm
{
    struct ParseError : std::logic_error
    {
        explicit ParseError(const std::string& msg)
            : logic_error(msg) {}

        explicit ParseError(const char* msg)
            : logic_error(msg) {}
    };

    class PrattParser
    {
    public:
        explicit PrattParser(Lexer& lexer);

        void registerParselet(TokenType type, std::unique_ptr<parselets::PrefixParselet> parselet);
        void registerParselet(TokenType type, std::unique_ptr<parselets::InfixParselet> parselet);
        [[nodiscard]]
        expressions::ExpressionPtr parseExpression(int precedence = 0);
        bool match(TokenType expected);
        Token consume(TokenType expected);
        Token consume();

    private:
        Lexer& lexer_;
        std::vector<Token> tokens_;
        std::map<TokenType, std::unique_ptr<parselets::PrefixParselet>> prefixParselets_;
        std::map<TokenType, std::unique_ptr<parselets::InfixParselet>> infixParselets_;

        Token lookAhead(std::size_t distance = 1);
        int getPrecedence();
    };
}

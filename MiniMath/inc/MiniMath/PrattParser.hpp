#pragma once

#include "Lexer.hpp"
#include "Parselets/StmtParselet.hpp"
#include "Parselets/PrefixParselet.hpp"
#include "Parselets/InfixParselet.hpp"
#include "Statements/Stmt.hpp"

#include <map>
#include <memory>
#include <stdexcept>
#include <vector>
#include <optional>

namespace mm
{
    struct ParseError : std::logic_error
    {
        using std::logic_error::logic_error;
    };

    class PrattParser
    {
    public:
        [[nodiscard]]
        explicit PrattParser(std::function<Token()> tokenSource);

        [[nodiscard]]
        Stmt parseStatement();
        [[nodiscard]]
        Expr parseExpression(int precedence = 0);
        [[nodiscard]]
        std::optional<Token> match(TokenType expected);        
        Token consume(TokenType expected);
        Token consume();
        void resetState();

    protected:
        void registerParselet(TokenType type, std::unique_ptr<parselets::PrefixParselet> parselet);
        void registerParselet(TokenType type, std::unique_ptr<parselets::InfixParselet> parselet);
        void registerParselet(TokenType type, std::unique_ptr<parselets::StmtParselet> parselet);

    private:
        std::function<Token()> tokenSource_;
        std::vector<Token> tokens_;
        std::map<TokenType, std::unique_ptr<parselets::StmtParselet>> stmtParselets_;
        std::map<TokenType, std::unique_ptr<parselets::PrefixParselet>> prefixParselets_;
        std::map<TokenType, std::unique_ptr<parselets::InfixParselet>> infixParselets_;

        Token lookAhead(std::size_t distance = 1);
        int getPrecedence();
    };
}

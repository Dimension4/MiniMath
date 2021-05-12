﻿#pragma once

#include "Lexer.hpp"
#include "Parselets/StmtParselet.hpp"
#include "Parselets/PrefixParselet.hpp"
#include "Parselets/InfixParselet.hpp"
#include "Statements/Stmt.hpp"

#include <map>
#include <memory>
#include <stdexcept>
#include <vector>


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
        explicit PrattParser(Lexer& lexer);

        [[nodiscard]]
        Stmt parseStatement();
        [[nodiscard]]
        Expr parseExpression(int precedence = 0);
        [[nodiscard]]
        bool match(TokenType expected);
        Token consume(TokenType expected);
        Token consume();
        void resetState();

    protected:
        void registerParselet(TokenType type, std::unique_ptr<parselets::PrefixParselet> parselet);
        void registerParselet(TokenType type, std::unique_ptr<parselets::InfixParselet> parselet);
        void registerParselet(TokenType type, std::unique_ptr<parselets::StmtParselet> parselet);

    private:
        Lexer& lexer_;
        std::vector<Token> tokens_;
        std::map<TokenType, std::unique_ptr<parselets::StmtParselet>> stmtParselets_;
        std::map<TokenType, std::unique_ptr<parselets::PrefixParselet>> prefixParselets_;
        std::map<TokenType, std::unique_ptr<parselets::InfixParselet>> infixParselets_;

        Token lookAhead(std::size_t distance = 1);
        int getPrecedence();
    };
}

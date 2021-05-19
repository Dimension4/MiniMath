#pragma once

#include "Token.hpp"

#include <functional>
#include <optional>
#include <stdexcept>

namespace mm
{
    class Lexer
    {
    public:
        [[nodiscard]]
        explicit Lexer(std::function<char()> charSource);

        Token nextToken();
        [[nodiscard]] bool atEof() const noexcept;
        void reset() noexcept;

    private:
        std::function<char()> charSource_;
        std::optional<char> overScan_;
        bool eof_ = false;

        char nextChar();
        Token readNumber(char first);
        Token readIdentifier(char first);
        std::optional<Token> tryReadWidePunctuator(char first);
        std::optional<Token> match(char c, char const(&lexeme)[3], TokenType type);
        bool skipComment(char c);
    };

    struct LexError : std::logic_error
    {
        using std::logic_error::logic_error;
    };
}

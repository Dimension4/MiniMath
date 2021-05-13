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
        bool atEof() const noexcept;
        void reset() noexcept;

    private:
        std::function<char()> charSource_;
        std::optional<char> overScan_;
        bool eof_ = false;

        char nextChar();
        Token readNumber(char first);
        Token readIdentifier(char first);
        std::optional<Token> tryReadWideToken(char c);
        static Token asKeyword(Token const& token);
    };

    struct LexError : std::logic_error
    {
        using std::logic_error::logic_error;
    };
}

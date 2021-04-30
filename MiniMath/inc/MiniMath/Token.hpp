#pragma once

#include <string_view>

namespace mm
{
    enum class TokenType
    {
        Invalid = -1,
        Identifier = 0,
        Number,
        LParen,
        RParen,
        Comma,
        Plus,
        Minus,
        Asterisk,
        Slash,
        Eof
    };

    struct Token
    {
        TokenType type = TokenType::Invalid;
        std::string_view lexeme;
    };

    constexpr std::string_view tokenName(TokenType type) noexcept
    {
#define TOKEN_CASE(x) case TokenType::x: return #x

        switch (type)
        {
        TOKEN_CASE(Invalid);
        TOKEN_CASE(Identifier);
        TOKEN_CASE(Number);
        TOKEN_CASE(LParen);
        TOKEN_CASE(RParen);
        TOKEN_CASE(Comma);
        TOKEN_CASE(Plus);
        TOKEN_CASE(Minus);
        TOKEN_CASE(Asterisk);
        TOKEN_CASE(Slash);
        TOKEN_CASE(Eof);
        default: return "<BAD>";
        }

#undef TOKEN_CASE
    }
}

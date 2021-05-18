#pragma once

#include <string_view>

namespace mm
{
    enum class TokenType
    {
        Identifier,
        Number,
        LParen,
        RParen,
        Comma,
        Plus,
        Minus,
        Asterisk,
        Slash,
        Equals,
        Let,
        In,
        Fn,
        RArrow,
        Import,
        Dot,
        True,
        False,
        Eof,
    };

    struct Token
    {
        TokenType type{};
        std::string lexeme;

        friend bool operator==(Token const&, Token const&) = default;
    };

    [[nodiscard]]
    constexpr std::string_view enumName(TokenType type) noexcept
    {
        switch (type)
        {
#define ENUM_CASE(x) case TokenType::x: return #x

        ENUM_CASE(Identifier);
        ENUM_CASE(Number);
        ENUM_CASE(LParen);
        ENUM_CASE(RParen);
        ENUM_CASE(Comma);
        ENUM_CASE(Plus);
        ENUM_CASE(Minus);
        ENUM_CASE(Asterisk);
        ENUM_CASE(Slash);
        ENUM_CASE(Equals);
        ENUM_CASE(Let);
        ENUM_CASE(In);
        ENUM_CASE(Fn);
        ENUM_CASE(RArrow);
        ENUM_CASE(Import);
        ENUM_CASE(Dot);
        ENUM_CASE(True);
        ENUM_CASE(False);
        ENUM_CASE(Eof);

#undef ENUM_CASE
        }

        return "<BAD>";
    }

    [[nodiscard]]
    constexpr std::string_view enumDisplay(TokenType type) noexcept
    {
        switch (type)
        {
        case TokenType::LParen: return "(";
        case TokenType::RParen: return ")";
        case TokenType::Comma: return ",";
        case TokenType::Plus: return "+";
        case TokenType::Minus: return "-";
        case TokenType::Asterisk: return "*";
        case TokenType::Slash: return "/";
        case TokenType::Equals: return "=";
        case TokenType::Let: return "let";
        case TokenType::In: return "in";
        case TokenType::Fn: return "fn";
        case TokenType::RArrow: return "->";
        case TokenType::Import: return "import";
        case TokenType::Dot: return ".";
        case TokenType::True: return "true";
        case TokenType::False: return "false";
        }

        return enumName(type);
    }
}

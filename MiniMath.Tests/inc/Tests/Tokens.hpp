#pragma once

#include "MiniMath/Token.hpp"

namespace tokconsts
{
    inline const mm::Token lparen{ mm::TokenType::LParen, "(" };
    inline const mm::Token rparen{ mm::TokenType::RParen, ")" };
    inline const mm::Token plus{ mm::TokenType::Plus, "+" };
    inline const mm::Token minus{ mm::TokenType::Minus, "-" };
    inline const mm::Token asterisk{ mm::TokenType::Asterisk, "*" };
    inline const mm::Token slash{ mm::TokenType::Slash, "/" };
    inline const mm::Token let{ mm::TokenType::Let, "let" };
    inline const mm::Token in{ mm::TokenType::In, "in" };
    inline const mm::Token equals{ mm::TokenType::Equals, "=" };
    inline const mm::Token fn{ mm::TokenType::Fn, "fn" };
    inline const mm::Token rarrow{ mm::TokenType::RArrow, "->" };
    inline const mm::Token import_{ mm::TokenType::Import, "import" };
    inline const mm::Token dot{ mm::TokenType::Dot, "." };
    inline const mm::Token true_{ mm::TokenType::True, "true" };
    inline const mm::Token false_{ mm::TokenType::False, "false" };
    inline const mm::Token eof{ mm::TokenType::Eof };

    inline mm::Token operator""_num(char const* val, std::size_t)
    {
        return { mm::TokenType::Number, val };
    }

    inline mm::Token operator""_id(char const* name, std::size_t)
    {
        return { mm::TokenType::Identifier, name };
    }
}

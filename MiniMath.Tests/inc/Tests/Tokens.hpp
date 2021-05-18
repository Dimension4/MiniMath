#pragma once

#include "MiniMath/Token.hpp"

namespace tokconsts
{
    inline mm::Token lparen{ mm::TokenType::LParen, "(" };
    inline mm::Token rparen{ mm::TokenType::RParen, ")" };
    inline mm::Token plus{ mm::TokenType::Plus, "+" };
    inline mm::Token minus{ mm::TokenType::Minus, "-" };
    inline mm::Token asterisk{ mm::TokenType::Asterisk, "*" };
    inline mm::Token slash{ mm::TokenType::Slash, "/" };
    inline mm::Token let{ mm::TokenType::Let, "let" };
    inline mm::Token in{ mm::TokenType::In, "in" };
    inline mm::Token equals{ mm::TokenType::Equals, "=" };
    inline mm::Token fn{ mm::TokenType::Fn, "fn" };
    inline mm::Token rarrow{ mm::TokenType::RArrow, "->" };
    inline mm::Token import_{ mm::TokenType::Import, "import" };
    inline mm::Token dot{ mm::TokenType::Dot, "." };
    inline mm::Token true_{ mm::TokenType::True, "true" };
    inline mm::Token false_{ mm::TokenType::False, "false" };

    inline mm::Token operator""_num(char const* val, std::size_t)
    {
        return { mm::TokenType::Number, val };
    }

    inline mm::Token operator""_id(char const* name, std::size_t)
    {
        return { mm::TokenType::Identifier, name };
    }
}

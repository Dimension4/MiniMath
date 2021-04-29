#pragma once

#include "Token.hpp"

#include <string>
#include <string_view>

namespace mm
{
    class Lexer
    {
    public:
        explicit Lexer(std::string_view source);
        Token nextToken();

    private:
        std::string_view source_;
        std::size_t pos_ = 0;

        Token readNumber();
        Token readIdentifier();
    };
}

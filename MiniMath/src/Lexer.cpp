#include "MiniMath/Lexer.hpp"

#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace mm
{
    static TokenType readPunctuator(char c)
    {
        switch (c)
        {
        case '+': return TokenType::Plus;
        case '-': return TokenType::Minus;
        case '*': return TokenType::Asterisk;
        case '/': return TokenType::Slash;
        case '(': return TokenType::LParen;
        case ')': return TokenType::RParen;
        case ',': return TokenType::Comma;
        case '=': return TokenType::Equals;
        default: return TokenType::Invalid;
        }
    }

    Lexer::Lexer(std::string_view source) : source_(source) { }

    Token Lexer::nextToken()
    {
        while (pos_ < source_.length())
        {
            auto c = source_[pos_];

            if (std::isspace(c))
            {
                pos_++;
                continue;
            }

            if (std::isdigit(c))
                return readNumber();

            if (std::isalpha(c))
                return asKeyword(readIdentifier());

            pos_++;

            if (auto type = readPunctuator(c); type != TokenType::Invalid)
            {
                return {type, source_.substr(pos_ - 1, 1)};
            }

            throw std::logic_error("Invalid character '" + std::string(1, c) + "' at position " + std::to_string(pos_ - 1) + ".");
        }

        return {.type = TokenType::Eof, .lexeme = source_.substr(source_.length())};
    }

    Token Lexer::readNumber()
    {
        auto begin = source_.data() + pos_;
        char* end;

        std::ignore = std::strtod(begin, &end);
        auto size = end - begin;
        pos_ += size;
        return {TokenType::Number, std::string_view(begin, size)};
    }

    Token Lexer::readIdentifier()
    {
        std::size_t start = pos_;
        while (pos_ < source_.length() && std::isalnum(source_[pos_]))
            pos_++;

        return {TokenType::Identifier, source_.substr(start, pos_ - start)};
    }

    Token Lexer::asKeyword(Token const& token)
    {
        if (token.lexeme == "let")
            return {TokenType::Let, token.lexeme};

        if (token.lexeme == "in")
            return {TokenType::In, token.lexeme};

        return token;
    }
}

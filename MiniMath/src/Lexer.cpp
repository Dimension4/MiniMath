#include "MiniMath/Lexer.hpp"

#include <fmt/format.h>

#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace mm
{
    static std::optional<TokenType> readPunctuator(char c)
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
        case '.': return TokenType::Dot;
        }

        return std::nullopt;
    }

    Lexer::Lexer(std::function<char()> charSource) : charSource_(move(charSource))
    {
    }

    Token Lexer::nextToken()
    {
        while (char c = nextChar())
        {
            if (skipComment(c))
                continue;

            if (std::isspace(c))
                continue;

            if (std::isdigit(c))
                return readNumber(c);

            if (std::isalpha(c))
                return asKeyword(readIdentifier(c));

            if (auto t = tryReadWideToken(c))
                return std::move(*t);

            if (auto type = readPunctuator(c))
                return { .type = *type, .lexeme = std::string(1, c) };

            throw LexError(fmt::format("Invalid character '{}'", c));
        }

        eof_ = true;
        return { .type = TokenType::Eof, .lexeme = "" };
    }

    bool Lexer::atEof() const noexcept
    {
        return eof_;
    }

    void Lexer::reset() noexcept
    {
        eof_ = false;
        overScan_.reset();
    }

    char Lexer::nextChar()
    {
        if (eof_)
            return 0;

        if (overScan_)
            return *std::exchange(overScan_, std::nullopt);

        return charSource_();
    }

    Token Lexer::readNumber(char first)
    {
        std::string buffer(1, first);
        char c = first;

        auto readDigits = [&]
        {
            while (std::isdigit(c = nextChar()))
                buffer += c;
        };

        readDigits();

        if (c == '.')
        {
            buffer += c;
            readDigits();
        }

        if (c == 'e' || c == 'E')
        {
            buffer += c;
            c = nextChar();

            if (c == '+' || c == '-')
                buffer += c;
            else
                overScan_ = c;

            readDigits();
        }

        overScan_ = c;

        return { .type = TokenType::Number, .lexeme = move(buffer) };
    }

    Token Lexer::readIdentifier(char first)
    {
        std::string buffer(1, first);
        char c = first;

        while (std::isalnum(c = nextChar()))
            buffer += c;

        overScan_ = c;

        return { .type = TokenType::Identifier, .lexeme = move(buffer) };
    }

    std::optional<Token> Lexer::tryReadWideToken(char c)
    {
        if (c == '-')
        {
            c = nextChar();
            if (c == '>')
                return Token{ .type = TokenType::RArrow, .lexeme = "->" };
            overScan_ = c;
        }

        return std::nullopt;
    }

    bool Lexer::skipComment(char c)
    {
        if (c != '#')
            return false;

        while (c && c != '\n' && c != '\r')
            c = nextChar();

        overScan_ = c;
        return true;
    }

    Token Lexer::asKeyword(Token const& token)
    {
        if (token.lexeme == "let")
            return { TokenType::Let, token.lexeme };

        if (token.lexeme == "in")
            return { TokenType::In, token.lexeme };

        if (token.lexeme == "fn")
            return { TokenType::Fn, token.lexeme };

        if (token.lexeme == "import")
            return { TokenType::Import, token.lexeme };

        return token;
    }
}

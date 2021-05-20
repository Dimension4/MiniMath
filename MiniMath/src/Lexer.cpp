#include "MiniMath/Lexer.hpp"

#include <fmt/format.h>

#include <cctype>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace mm
{
    static TokenType classifyIdentifier(std::string_view identifier)
    {
        if (identifier == "let") return TokenType::Let;
        if (identifier == "in") return TokenType::In;
        if (identifier == "fn") return TokenType::Fn;
        if (identifier == "import") return TokenType::Import;
        if (identifier == "true") return TokenType::True;
        if (identifier == "false") return TokenType::False;
        if (identifier == "and") return TokenType::And;
        if (identifier == "or") return TokenType::Or;
        if (identifier == "if") return TokenType::If;
        if (identifier == "then") return TokenType::Then;
        if (identifier == "else") return TokenType::Else;

        return TokenType::Identifier;
    }

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
        case '<': return TokenType::Less;
        case '>': return TokenType::Greater;
        }

        return std::nullopt;
    }

    Lexer::Lexer(std::function<char()> charSource) : charSource_(move(charSource)) { }

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
            {
                auto lexeme = readIdentifier(c);
                auto type = classifyIdentifier(lexeme);
                return { type, move(lexeme) };
            }

            if (auto t = tryReadWidePunctuator(c))
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

    std::string Lexer::readIdentifier(char first)
    {
        std::string lexeme(1, first);
        char c = first;

        while (std::isalnum(c = nextChar()))
            lexeme += c;

        overScan_ = c;

        return lexeme;
    }

    std::optional<Token> Lexer::tryReadWidePunctuator(char first)
    {
        constexpr std::pair<char const(&)[3], TokenType> arr[] = {
            { "->", TokenType::RArrow },
            { "<=", TokenType::LessEqual },
            { ">=", TokenType::GreaterEqual },
        };

        for (auto&& [lexeme, type] : arr)
            if (auto t = match(first, lexeme, type))
                return t;

        return std::nullopt;
    }

    std::optional<Token> Lexer::match(char c, char const (& lexeme)[3], TokenType type)
    {
        if (c == lexeme[0])
        {
            c = nextChar();
            if (c == lexeme[1])
                return Token{ .type = type, .lexeme = lexeme };
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
}

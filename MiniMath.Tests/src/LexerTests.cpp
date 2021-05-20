#include "Tests/Tokens.hpp"

#include <MiniMath/Lexer.hpp>

#include <catch2/catch.hpp>


using namespace tokconsts;

namespace mm::tests::lexer
{
    static auto makeLexer(std::string_view str)
    {
        return Lexer([=, pos = 0ull]() mutable
        {
            return pos < str.size() ? str[pos++] : 0;
        });
    }

    TEST_CASE("lexing illegal character throws LexError")
    {
        auto lexer = makeLexer("?");
        REQUIRE_THROWS_AS(lexer.nextToken(), LexError);
    }

    TEST_CASE("empty string produces only EOF")
    {
        auto lexer = makeLexer("");
        auto token = lexer.nextToken();

        REQUIRE(token == eof);
        REQUIRE(token.lexeme.empty());
    }

    TEST_CASE("strings lex to identifiers")
    {
        auto lexer = makeLexer("hello world");
        auto hello = lexer.nextToken();
        auto world = lexer.nextToken();

        Token expectedHello{ TokenType::Identifier, "hello" };
        Token expectedWorld{ TokenType::Identifier, "world" };

        REQUIRE(lexer.nextToken() == eof);
        REQUIRE(hello == expectedHello);
        REQUIRE(world == expectedWorld);
    }

    TEST_CASE("numbers lex to numbers")
    {
        std::string_view source = GENERATE("0", "123", "123.23", "1e2", "3e-2", "1.2e3", "1.2e-34");

        auto lexer = makeLexer(source);
        auto number = lexer.nextToken();
        Token expected{ TokenType::Number, std::string(source) };

        REQUIRE(lexer.nextToken() == eof);
        REQUIRE(number == expected);
    }

    TEST_CASE("comments are ignored")
    {
        std::string_view source = "12# dajod2ß..\"let in import";

        auto lexer = makeLexer(source);
        REQUIRE(lexer.nextToken() == Token{ TokenType::Number, "12" });
        REQUIRE(lexer.nextToken() == eof);
    }

    TEST_CASE("comments are ignored until EOL")
    {
        std::string_view source = R"(12# dajod2ß..let in import
        hi)";

        auto lexer = makeLexer(source);
        REQUIRE(lexer.nextToken() == Token{ TokenType::Number, "12" });
        REQUIRE(lexer.nextToken() == Token{ TokenType::Identifier, "hi" });
        REQUIRE(lexer.nextToken() == eof);
    }

    TEST_CASE("keywords lex to keywords")
    {
        auto token = GENERATE(let, in, fn, import_, true_, false_, and_, or_, if_, then_, else_);

        auto lexer = makeLexer(token.lexeme);
        REQUIRE(lexer.nextToken() == token);
        REQUIRE(lexer.nextToken() == eof);
    }

    TEST_CASE("punctuators lex as punctuators")
    {
        auto token = GENERATE(lparen, rparen, plus, minus, asterisk, slash, equals, rarrow, dot, le, lt, gt, ge);

        auto lexer = makeLexer(token.lexeme);
        REQUIRE(lexer.nextToken() == token);
        REQUIRE(lexer.nextToken() == eof);
    }
}

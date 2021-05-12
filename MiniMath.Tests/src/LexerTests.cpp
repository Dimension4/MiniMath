#include <MiniMath/Lexer.hpp>
#include <catch2/catch.hpp>


auto makeLexer(std::string_view str)
{
    return mm::Lexer([=, pos = 0ull]() mutable
    {
        return pos < str.size() ? str[pos++] : 0;
    });
}

constexpr bool operator==(const mm::Token& lhs, const mm::Token& rhs)
{
    return lhs.type == rhs.type && lhs.lexeme == rhs.lexeme;
}

namespace mm::tests
{
    TEST_CASE("empty string produces only EOF")
    {
        auto lexer = makeLexer("");
        auto token = lexer.nextToken();

        REQUIRE(token.type == TokenType::Eof);
        REQUIRE(token.lexeme.empty());
    }

    TEST_CASE("identifiers only")
    {
        auto lexer = makeLexer("hello world");
        auto hello = lexer.nextToken();
        auto world = lexer.nextToken();

        Token expectedHello{ TokenType::Identifier, "hello" };
        Token expectedWorld{ TokenType::Identifier, "world" };

        REQUIRE(lexer.nextToken().type == TokenType::Eof);
        REQUIRE(hello == expectedHello);
        REQUIRE(world == expectedWorld);
    }

    TEST_CASE("single number")
    {
        std::string_view source = GENERATE("0", "123", "123.23", "1e2", "3e-2", "1.2e3", "1.2e-34");

        auto lexer = makeLexer(source);
        auto number = lexer.nextToken();
        Token expected{ TokenType::Number, std::string(source) };

        REQUIRE(lexer.nextToken().type == TokenType::Eof);
        REQUIRE(number == expected);
    }
}

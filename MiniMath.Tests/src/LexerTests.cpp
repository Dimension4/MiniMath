#include <MiniMath/Lexer.hpp>
#include <catch2/catch.hpp>


constexpr bool operator==(const mm::Token& lhs, const mm::Token& rhs)
{
    return lhs.type == rhs.type
        && lhs.lexeme.data() == rhs.lexeme.data()
        && lhs.lexeme.length() == rhs.lexeme.length();
}

namespace mm::tests
{
    TEST_CASE("empty string produces only EOF")
    {
        Lexer lexer("");
        auto token = lexer.nextToken();

        REQUIRE(token.type == TokenType::Eof);
        REQUIRE(token.lexeme == "");
    }

    TEST_CASE("identifiers only")
    {
        std::string_view source = "hello world";

        Lexer lexer(source);
        auto hello = lexer.nextToken();
        auto world = lexer.nextToken();

        Token expectedHello{TokenType::Identifier, std::string(source.substr(0, 5))};
        Token expectedWorld{TokenType::Identifier, std::string(source.substr(6))};

        REQUIRE(lexer.nextToken().type == TokenType::Eof);
        REQUIRE(hello == expectedHello);
        REQUIRE(world == expectedWorld);
    }

    TEST_CASE("single number")
    {
        std::string_view source = GENERATE("0", "123", "123.23", "1e2", "3e-2", "1.2e3", "1.2e-34");

        Lexer lexer(source);
        auto number = lexer.nextToken();
        Token expected{TokenType::Number, std::string(source.substr(0))};

        REQUIRE(lexer.nextToken().type == TokenType::Eof);
        REQUIRE(number == expected);
    }
}

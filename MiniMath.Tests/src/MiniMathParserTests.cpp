#include "Tests/Tokens.hpp"
#include "Tests/Expressions.hpp"

#include <MiniMath/MiniMath.hpp>

#include <catch2/catch.hpp>

#include <utility>
#include <ostream>

using namespace tokconsts;
using namespace expr;

static auto tokens(std::vector<mm::Token> const& tokens)
{
    return [=, i = 0]() mutable
    {
        return i < tokens.size()
                   ? tokens[i++]
                   : mm::Token{ mm::TokenType::Eof };
    };
}

namespace mm
{
    static std::ostream& operator<<(std::ostream& os, Expr const& expr)
    {
        return os << fmt::format("{}", expr);
    }
}


namespace mm::tests::parser
{
    using namespace expr;
    using namespace stmt;

    TEST_CASE("only EOF, parseExpression throws ParseError")
    {
        MiniMathParser parser(tokens({}));
        REQUIRE_THROWS_AS(parser.parseExpression(), ParseError);
    }

    TEST_CASE("only EOF, parseStatement throws ParseError")
    {
        MiniMathParser parser(tokens({}));
        REQUIRE_THROWS_AS(parser.parseStatement(), ParseError);
    }

    TEST_CASE("parse numbers")
    {
        auto num = GENERATE("1", "1.2", "1e2", "1E2", "12.34e2", "12.32e-3");
        auto expected = std::strtod(num, nullptr);

        MiniMathParser parser(tokens({ Token{ .type = TokenType::Number, .lexeme = num } }));
        auto e = parser.parseExpression();

        REQUIRE(e == makeExpr(ConstantExpr{ expected }));
    }

    TEST_CASE("parse names")
    {
        auto id = GENERATE("hello", "bla", "i2", "REE", "u2u");

        MiniMathParser parser(tokens({ Token{ .type = TokenType::Identifier, .lexeme = id } }));
        auto e = parser.parseExpression();

        REQUIRE(e == makeExpr(NameExpr{ id }));
    }

    TEST_CASE("parse short let expression")
    {
        auto source = tokens({ let, "x"_id, equals, "1"_num, "x"_id });

        MiniMathParser parser(source);

        auto actual = parser.parseExpression();
        auto expected = makeExpr(LetExpr{
            .name = "x",
            .value = ConstantExpr{ 1 },
            .body = NameExpr{ "x" }
        });

        REQUIRE(actual == expected);
    }

    TEST_CASE("parse let expression")
    {
        auto source = tokens({ let, "x"_id, equals, "1"_num, in, "x"_id });

        MiniMathParser parser(source);

        auto actual = parser.parseExpression();
        auto expected = makeExpr(LetExpr{
            .name = "x",
            .value = ConstantExpr{ 1 },
            .body = NameExpr{ "x" }
        });

        REQUIRE(actual == expected);
    }

    TEST_CASE("parse let statement")
    {
        auto source = tokens({ let, "x"_id, equals, "1"_num });

        MiniMathParser parser(source);

        auto actual = parser.parseStatement();
        auto expected = Stmt{ LetStmt{ .name = "x", .value = ConstantExpr{ 1 } } };

        REQUIRE(actual == expected);
    }

    TEST_CASE("parse function expression")
    {
        auto source = tokens({ fn, "a"_id, "b"_id, rarrow, "a"_id });

        MiniMathParser parser(source);

        auto actual = parser.parseExpression();
        auto expected = makeExpr(FunctionExpr{
            .paramNames = { "a", "b" },
            .body = NameExpr{ "a" }
        });

        REQUIRE(actual == expected);
    }

    TEST_CASE("parse binary expression")
    {
        using p = std::pair<Token, BinaryOperation>;
        auto [token, op] = GENERATE(
            p(plus, BinaryOperation::Add),
            p(minus, BinaryOperation::Subtract),
            p(asterisk, BinaryOperation::Multiply),
            p(slash, BinaryOperation::Divide)
        );

        auto source = tokens({
            Token{ TokenType::Number, "1" },
            token,
            Token{ TokenType::Number, "2" },
        });

        MiniMathParser parser(source);

        auto actual = parser.parseExpression();
        auto expected = makeExpr(BinaryExpr{
            .operation = op,
            .left = ConstantExpr{ 1 },
            .right = ConstantExpr{ 2 }
        });

        REQUIRE(actual == expected);
    }

    TEST_CASE("parse call expression")
    {
        auto source = tokens({ "foo"_id, lparen, "x"_id, "2"_num, rparen });

        MiniMathParser parser(source);

        auto actual = parser.parseExpression();
        auto expected = makeExpr(CallExpr{
            .target = NameExpr{ "foo" },
            .args = { NameExpr{ "x" }, ConstantExpr{ 2 } }
        });

        REQUIRE(actual == expected);
    }

    namespace precedence
    {
        TEST_CASE("x + x * 2 >=> x + (x * 2)")
        {
            auto source = tokens({ "x"_id, plus, "x"_id, asterisk, "2"_num });

            MiniMathParser parser(source);

            auto actual = parser.parseExpression();
            auto expected =
                binexpr(BinaryOperation::Add,
                        "x"_name,
                        binexpr(
                            BinaryOperation::Multiply,
                            "x"_name,
                            2_const
                        ));

            REQUIRE(actual == expected);
        }

        TEST_CASE("1 + 2 - 3 >=> ((1 + 2) - 3)")
        {
            auto source = tokens({ "1"_num, plus, "2"_num, minus, "3"_num });

            MiniMathParser parser(source);

            auto actual = parser.parseExpression();
            auto expected =
                binexpr(BinaryOperation::Subtract,
                        binexpr(
                            BinaryOperation::Add,
                            1_const,
                            2_const
                        ),
                        3_const);
            
            REQUIRE(actual == expected);
        }
    }
}

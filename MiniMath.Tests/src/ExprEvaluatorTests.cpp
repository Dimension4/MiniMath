#include "Tests/Expressions.hpp"

#include "MiniMath/MiniMath.hpp"

#include <catch2/catch.hpp>

using namespace expr;

namespace mm::tests::evaluator
{
    using namespace ast;

    TEST_CASE("1 = 1")
    {
        Environment env;
        auto actual = evaluate(1_num, env);
        auto expected = 1_num;

        REQUIRE(actual == expected);
    }

    TEST_CASE("evaluate (x) raises LookupError")
    {
        Environment env;

        REQUIRE_THROWS_AS(evaluate("x"_name, env), LookupError);
    }

    TEST_CASE("evaluate (x) performs lookup")
    {
        Environment env;
        env.add("x", 1_num);

        auto actual = evaluate("x"_name, env);
        auto expected = 1_num;

        REQUIRE(actual == expected);
    }

    TEST_CASE("1 + 1 = 2")
    {
        Environment env;
        auto actual = evaluate(1_num + 1_num, env);
        auto expected = 2_num;

        REQUIRE(actual == expected);
    }

    TEST_CASE("[x = 2] x * x = 4")
    {
        Environment env;
        env.add("x", 2_num);

        auto actual = evaluate("x"_name * "x"_name, env);
        auto expected = 4_num;

        REQUIRE(actual == expected);
    }

    TEST_CASE("evaluate let expression")
    {
        Environment env;

        auto actual = evaluate(("x"_let = 3_num) >> ("x"_name * "x"_name), env);
        auto expected = 9_num;

        REQUIRE(actual == expected);
    }

    TEST_CASE("evaluate variable shadowing")
    {
        Environment env;
        env.add("x", 2_num);

        auto actual = evaluate(("x"_let = 3_num) >> ("x"_name * "x"_name), env);
        auto expected = 9_num;

        REQUIRE(actual == expected);
    }

    TEST_CASE("evaltuate function call")
    {
        Environment env;
        env.add("f", closure({ "x", "y" }, "x"_name + "y"_name));

        auto actual = evaluate(callexpr("f"_name, { 1_num, 2_num }), env);
        auto expected = 3_num;

        REQUIRE(actual == expected);
    }

    TEST_CASE("function call with too few arguments raises ArgumentError")
    {
        Environment env;
        env.add("f", closure({ "x", "y" }, "x"_name + "y"_name));

        REQUIRE_THROWS_AS(evaluate(callexpr("f"_name, { 1_num }), env), ArgumentError);
    }

    TEST_CASE("function call with too many arguments raises ArgumentError")
    {
        Environment env;
        env.add("f", closure({ "x", "y" }, "x"_name + "y"_name));

        REQUIRE_THROWS_AS(evaluate(callexpr("f"_name, { 1_num, 2_num, 3_num }), env), ArgumentError);
    }

    TEST_CASE("function call on non-function raises TypeError")
    {
        Environment env;
        env.add("f", 3_num);

        REQUIRE_THROWS_AS(evaluate(callexpr("f"_name, {}), env), TypeError);
    }

    TEST_CASE("evaluate chain calls: let f = fn x -> fn y -> x - y in f(3)(2)")
    {
        Environment env;

        auto actual = evaluate(
            ("f"_let = fnexpr({ "x" }, fnexpr({ "y" }, "x"_name - "y"_name)))
            >> callexpr(callexpr("f"_name, { 3_num }), { 2_num }),
            env);
        auto expected = 1_num;

        REQUIRE(actual == expected);
    }

    TEST_CASE("functions are evaluated to closures")
    {
        Environment env;

        auto actual = evaluate(fnexpr({ "x", "y" }, 1_num), env);
        auto expected = closure({ "x", "y" }, 1_num);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures capture lexical scope")
    {
        Environment env;
        env.add("a", 1_num);

        auto actual = evaluate(fnexpr({ "x", "y" }, "a"_name), env);
        auto expected = closure({ "x", "y" }, "a"_name, env);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures capture only used bindings (nothing)")
    {
        Environment env;
        env.add("a", 1_num);

        auto actual = evaluate(fnexpr({ "x", "y" }, "x"_name), env);
        auto expected = closure({ "x", "y" }, "x"_name);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures capture only used bindings (1 binding)")
    {
        Environment closureEnv;
        closureEnv.add("a", 1_num);

        auto env = closureEnv.with("b", 2_num);

        auto actual = evaluate(fnexpr({ "x", "y" }, "a"_name), env);
        auto expected = closure({ "x", "y" }, "a"_name, closureEnv);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures don't capture when params shadow")
    {
        Environment env;
        env.add("x", 1_num);
        env.add("a", 2_num);

        auto actual = evaluate(fnexpr({ "x", "y" }, "x"_name), env);
        auto expected = closure({ "x", "y" }, "x"_name);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures don't capture when params shadow in nested functions")
    {
        Environment env;
        env.add("x", 1_num);
        env.add("a", 2_num);

        auto inner = fnexpr({ "a" }, "x"_name * "a"_name);

        auto actual = evaluate(fnexpr({ "x" }, inner), env);
        auto expected = closure({ "x" }, inner);

        REQUIRE(actual == expected);
    }

    TEST_CASE("-1 + -1 = -2")
    {
        Environment env;
        auto actual = evaluate(-1_num + -1_num, env);
        Expr expected = -2.0;

        REQUIRE(actual == expected);
    }

    TEST_CASE("[x = 4] -(-x + 2) = 2")
    {
        Environment env;
        env.add("x", 4_num);

        auto actual = evaluate(-(-"x"_name + 2_num), env);
        auto expected = 2_num;

        REQUIRE(actual == expected);
    }

    TEST_CASE("1 < 2 and 2 > 1")
    {
        Environment env;

        auto actual = evaluate(1_num < 2_num && 2_num > 1_num);
        Expr expected = true;

        REQUIRE(actual == expected);
    }

    TEST_CASE("true and false")
    {
        Environment env;

        auto actual = evaluate(Expr(true) && Expr(false));
        Expr expected = false;

        REQUIRE(actual == expected);
    }

    TEST_CASE("true or false")
    {
        Environment env;

        auto actual = evaluate(Expr(true) || Expr(false));
        Expr expected = true;

        REQUIRE(actual == expected);
    }

    TEST_CASE("(if 1 + 2 > 3 then 1 else 0) = 0")
    {
        Environment env;

        auto actual = evaluate(ifexpr(1_num + 2_num > 3_num, 1_num, 0_num));
        auto expected = 0_num;

        REQUIRE(actual == expected);
    }
}

﻿#include "Tests/Expressions.hpp"

#include "MiniMath/MiniMath.hpp"

#include <catch2/catch.hpp>

using namespace expr;

namespace mm::tests::evaluator
{
    using namespace ast;

    TEST_CASE("1 = 1")
    {
        Environment env;
        auto actual = evaluate(1_const, env);
        auto expected = 1_const;

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
        env.add("x", 1_const);

        auto actual = evaluate("x"_name, env);
        auto expected = 1_const;

        REQUIRE(actual == expected);
    }

    TEST_CASE("1 + 1 = 2")
    {
        Environment env;
        auto actual = evaluate(1_const + 1_const, env);
        auto expected = 2_const;

        REQUIRE(actual == expected);
    }

    TEST_CASE("[x = 2] x * x = 4")
    {
        Environment env;
        env.add("x", 2_const);

        auto actual = evaluate("x"_name * "x"_name, env);
        auto expected = 4_const;

        REQUIRE(actual == expected);
    }

    TEST_CASE("evaluate let expression")
    {
        Environment env;

        auto actual = evaluate(("x"_let = 3_const) >> ("x"_name * "x"_name), env);
        auto expected = 9_const;

        REQUIRE(actual == expected);
    }

    TEST_CASE("variable shadowing")
    {
        Environment env;
        env.add("x", 2_const);

        auto actual = evaluate(("x"_let = 3_const) >> ("x"_name * "x"_name), env);
        auto expected = 9_const;

        REQUIRE(actual == expected);
    }

    TEST_CASE("evaltuate function call")
    {
        Environment env;
        env.add("f", closure({ "x", "y" }, "x"_name + "y"_name));

        auto actual = evaluate(callexpr("f"_name, { 1_const, 2_const }), env);
        auto expected = 3_const;

        REQUIRE(actual == expected);
    }

    TEST_CASE("function call with too few arguments raises ArgumentError")
    {
        Environment env;
        env.add("f", closure({ "x", "y" }, "x"_name + "y"_name));

        REQUIRE_THROWS_AS(evaluate(callexpr("f"_name, { 1_const }), env), ArgumentError);
    }

    TEST_CASE("function call with too many arguments raises ArgumentError")
    {
        Environment env;
        env.add("f", closure({ "x", "y" }, "x"_name + "y"_name));

        REQUIRE_THROWS_AS(evaluate(callexpr("f"_name, { 1_const, 2_const, 3_const }), env), ArgumentError);
    }

    TEST_CASE("function call on non-function raises TypeError")
    {
        Environment env;
        env.add("f", 3_const);

        REQUIRE_THROWS_AS(evaluate(callexpr("f"_name, {}), env), TypeError);
    }

    TEST_CASE("evaluate chain calls: let f = fn x -> fn y -> x - y in f(3)(2)")
    {
        Environment env;

        auto actual = evaluate(
            ("f"_let = fnexpr({ "x" }, fnexpr({ "y" }, "x"_name - "y"_name)))
            >> callexpr(callexpr("f"_name, { 3_const }), { 2_const }),
            env);
        auto expected = 1_const;

        REQUIRE(actual == expected);
    }

    TEST_CASE("functions are evaluated to closures")
    {
        Environment env;

        auto actual = evaluate(fnexpr({ "x", "y" }, 1_const), env);
        auto expected = closure({ "x", "y" }, 1_const);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures capture lexical scope")
    {
        Environment env;
        env.add("a", 1_const);

        auto actual = evaluate(fnexpr({ "x", "y" }, "a"_name), env);
        auto expected = closure({ "x", "y" }, "a"_name, env);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures capture only used bindings (nothing)")
    {
        Environment env;
        env.add("a", 1_const);

        auto actual = evaluate(fnexpr({ "x", "y" }, "x"_name), env);
        auto expected = closure({ "x", "y" }, "x"_name);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures capture only used bindings (1 binding)")
    {
        Environment closureEnv;
        closureEnv.add("a", 1_const);

        auto env = closureEnv.with("b", 2_const);

        auto actual = evaluate(fnexpr({ "x", "y" }, "a"_name), env);
        auto expected = closure({ "x", "y" }, "a"_name, closureEnv);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures don't capture when params shadow")
    {
        Environment env;
        env.add("x", 1_const);
        env.add("a", 2_const);

        auto actual = evaluate(fnexpr({ "x", "y" }, "x"_name), env);
        auto expected = closure({ "x", "y" }, "x"_name);

        REQUIRE(actual == expected);
    }

    TEST_CASE("closures don't capture when params shadow in nested functions")
    {
        Environment env;
        env.add("x", 1_const);
        env.add("a", 2_const);

        auto inner = fnexpr({ "a" }, "x"_name * "a"_name);

        auto actual = evaluate(fnexpr({ "x" }, inner), env);
        auto expected = closure({ "x" }, inner);

        REQUIRE(actual == expected);
    }
}

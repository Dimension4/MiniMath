﻿#include "Tests/Expressions.hpp"

#include "MiniMath/MiniMath.hpp"

#include <catch2/catch.hpp>
#include <fmt/format.h>

using namespace expr;

namespace mm::ast
{
    std::ostream& operator<<(std::ostream& os, Environment const& env)
    {
        return os << fmt::format("{}", env);
    }
}

namespace mm::integration::tests
{
    using namespace ast;
    using namespace expr;

    Environment evalStatement(std::string_view source, Environment initial = {})
    {
        evaluate(initial, [&, i = 0u]() mutable { return i < source.length() ? source[i++] : 0; });
        return initial;
    }

    TEST_CASE("value and function let bindings")
    {
        constexpr std::string_view source = R"(
let x = 1 + 1 # adds 1 and 1
let plus1 = fn x -> x + 1

# here we can see nested bindings in action
let y =
    let x1 = x + 1
    plus1(x1)
)";

        Environment expected{
            { "x", 2_num },
            { "plus1", closure({ "x" }, "x"_name + 1_num) },
            { "y", 4_num }
        };

        auto actual = evalStatement(source);

        REQUIRE(actual == expected);
    }

    TEST_CASE("variable capture")
    {
        constexpr std::string_view source = R"(
let x = 22
let add = fn a b -> a + b
let plusX = fn y -> add(x y)
let y = plusX(2)
)";

        auto add = makeExpr(Closure{ .environment = {}, .paramNames = { "a", "b" }, .body = "a"_name + "b"_name });

        Environment expected{
            { "x", 22_num },
            { "add", add },
            { "plusX", closure({ "y" }, callexpr("add"_name, { "x"_name, "y"_name }), { { "x", 22_num }, { "add", add } }) },
            { "y", 24_num }
        };

        auto actual = evalStatement(source);

        REQUIRE(actual == expected);
    }

    TEST_CASE("variable shadowing")
    {
        constexpr std::string_view source = R"(
let x = 22
let x = 12
let y =
    let x1 = x
    let x = x1 + 1
    x
let x = 1
)";

        Environment expected{
            { "x", 1_num },
            { "y", 13_num }
        };

        auto actual = evalStatement(source);

        REQUIRE(actual == expected);
    }

    TEST_CASE("functions as arguments")
    {
        constexpr std::string_view source = R"(
let apply = fn func x -> func(x)
let add = fn a -> fn b -> a + b
let x = apply(add(2) 2)
)";

        Environment expected{
            { "apply", closure({ "func", "x" }, callexpr("func"_name, { "x"_name })) },
            { "add", closure({ "a" }, fnexpr({ "b" }, "a"_name + "b"_name)) },
            { "x", 4_num }
        };

        auto actual = evalStatement(source);

        REQUIRE(actual == expected);
    }

    TEST_CASE("complex functions")
    {
        constexpr std::string_view source = R"(
# newton's method
let newton = fn x f fd -> x - f(x) / fd(x)

# a random polynomial with its derivative
let f = fn x -> x * x * x - 18 * x * x + 100 * x - 164
let fd = fn x -> 3 * x * x - 36 * x + 100

let root = fn x -> newton(x f fd)

let r1 = root(root(root(root(0)))) # first root @ 2.9486
let r2 = root(root(root(root(5)))) # second root @ 6.5173
let r3 = root(root(root(root(10)))) # third root @ 8.5341
)";

        NumberExpr r1 = 2.9486;
        NumberExpr r2 = 6.5173;
        NumberExpr r3 = 8.5341;

        auto actual = evalStatement(source);

        auto check = [&](std::string_view name, NumberExpr exp)
        {
            auto act = actual.tryGetAs<NumberExpr>(name);
            REQUIRE(act);
            CHECK(*act == Approx(exp).epsilon(0.01));
        };

        check("r1", r1);
        check("r2", r2);
        check("r3", r3);
    }

    TEST_CASE("negative values")
    {
        constexpr std::string_view source = R"(
let x = -1 + 2
let y =
    let a = x - 1 in
    -a + 2
    # we need the 'in' keyword here because of ambiguity
    # since we don't care about whitespaces,
    # let x = 1
    # -x
    # is interpreted as 'let x = (1 -x) (missing-body)', which is obviously
    # not the intended behavior given the line break. YOLO

let z = --y

let gotcha =
    let a = 1
    let b = 2
    let f = fn x -> -a + b
    f(0)
)";

        Environment expected{
            { "x", 1_num },
            { "y", 2_num },
            { "z", 2_num },
            { "gotcha", 1_num },
        };

        auto actual = evalStatement(source);

        REQUIRE(actual == expected);
    }

    TEST_CASE("logical operations")
    {
        constexpr std::string_view source = R"(
let t = true
let f = false

let a = t and f
let b = t or f
let c = f and t or t and t
let d = f or t and f or f
)";

        Environment expected{
            { "t", true },
            { "f", false },
            { "a", false },
            { "b", true },
            { "c", true },
            { "d", false },
        };

        auto actual = evalStatement(source);

        REQUIRE(actual == expected);
    }

    TEST_CASE("relational operations")
    {
        constexpr std::string_view source = R"(
let a = 1 < 2
let b = 2 <= 2
let c = 2 > 1
let d = 2 >= 2
let e = 2 = 2
let f = 1 = 2

let fun = fn x -> (x + 2) * 4 > 20
let g = fun(2) = false
let h = fun(3) = false
let i = fun(4) = true

let x = 1 < 2 and 2 > 1 or 4 <= 4 and 4 >= 4
let y = x = (1 = 2)
)";

        Environment expected{
            { "a", true },
            { "b", true },
            { "c", true },
            { "d", true },
            { "e", true },
            { "f", false },
            { "x", true },
            { "y", false },
            { "fun", closure({ "x" }, ("x"_name + 2_num) * 4_num > 20_num) },
            { "g", true },
            { "h", true },
            { "i", true },
        };

        auto actual = evalStatement(source);

        REQUIRE(actual == expected);
    }

    TEST_CASE("if expressions")
    {
        constexpr std::string_view source = R"(
let a = if true then 1 else 2
let b = if 2 * 2 < 3 then 1 + 1 else 2 * 2
let c =
    if
        let x = 2
        let y = 4
        x * -y < -x * y
    then
        a
    else
        b

let d =
    let f = fn a b -> if a >= b then a - b else b - a
    if f(2 4) = f(4 2) then 1 else 0

let e =
    let f = fn x -> if x >= 0 then false else -1
    if f(0)
    then if f(-1) < 0 then 1 else -1
    else -2
)";

        Environment expected{
            { "a", 1_num },
            { "b", 4_num },
            { "c", 4_num },
            { "d", 1_num },
            { "e", NumberExpr{-2} },
        };

        auto actual = evalStatement(source);

        REQUIRE(actual == expected);
    }
}

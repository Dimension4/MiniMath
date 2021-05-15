#pragma once

// ReSharper disable CppUnusedIncludeDirective

#include "Expr.hpp"
#include "BinaryExpr.hpp"
#include "MiniMath/Ast/Environment.hpp"

#include <vector>
#include <string>

namespace mm::expr
{
    struct CallExpr
    {
        Expr target;
        std::vector<Expr> args;

        friend bool operator==(CallExpr const&, CallExpr const&) = default;
    };

    struct Closure
    {
        ast::Environment environment;
        std::vector<std::string> paramNames;
        Expr body;

        friend bool operator==(Closure const&, Closure const&) = default;
    };

    struct FunctionExpr
    {
        std::vector<std::string> paramNames;
        Expr body;

        friend bool operator==(FunctionExpr const&, FunctionExpr const&) = default;
    };

    struct LetExpr
    {
        std::string name;
        Expr value;
        Expr body;

        friend bool operator==(LetExpr const&, LetExpr const&) = default;
    };
}

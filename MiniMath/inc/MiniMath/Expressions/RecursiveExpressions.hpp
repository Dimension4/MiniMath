#pragma once

// ReSharper disable CppUnusedIncludeDirective

#include "Expr.hpp"
#include "BinaryExpr.hpp"
#include "MiniMath/Ast/Environment.hpp"

#include <vector>
#include <string>

namespace mm::expressions
{
    struct CallExpr
    {
        Expr target;
        std::vector<Expr> args;
    };

    struct Closure
    {
        ast::Environment environment;
        std::vector<std::string> paramNames;
        Expr body;
    };

    struct LetExpr
    {
        std::string name;
        Expr value;
        Expr body;
    };
}

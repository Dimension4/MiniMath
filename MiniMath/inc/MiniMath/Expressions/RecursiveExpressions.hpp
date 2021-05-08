#pragma once

// ReSharper disable CppUnusedIncludeDirective

#include "Expr.hpp"
#include "BinaryExpr.hpp"

#include <vector>

namespace mm::expressions
{
    struct CallExpr
    {
        Expr target;
        std::vector<Expr> args;
    };
}

#pragma once

#include <string>

// Leaf expressions are expressions that have no sub expressions

namespace mm::expr
{
    struct NameExpr
    {
        std::string name;
    };

    struct ConstantExpr
    {
        double value;
    };
}
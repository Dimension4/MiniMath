#pragma once

#include <string>

// Leaf expressions are expressions that have no sub expressions

namespace mm::expr
{
    struct NameExpr
    {
        std::string name;

        friend bool operator==(NameExpr const&, NameExpr const&) = default;
    };

    using NumberExpr = double;
    using BoolExpr = bool;
}
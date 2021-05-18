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

    struct NumberExpr
    {
        double value;

        friend constexpr bool operator==(NumberExpr const&, NumberExpr const&) = default;
    };
}
#pragma once

namespace mm
{
    enum class OperatorPrecedence
    {
        None,

        Addition,
        Subtraction = Addition,

        Multiplication,
        Division = Multiplication,

        Negate,
    };
}

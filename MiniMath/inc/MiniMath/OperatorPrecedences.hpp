#pragma once

namespace mm
{
    enum class OperatorPrecedence
    {
        None,

        Or,
        And,
        Relational,

        Addition,
        Subtraction = Addition,

        Multiplication,
        Division = Multiplication,

        Prefix,
        Call,
    };
}

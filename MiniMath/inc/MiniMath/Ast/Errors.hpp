#pragma once

#include <stdexcept>

namespace mm::ast
{
    struct TypeError : std::logic_error
    {
        using std::logic_error::logic_error;
    };

    struct LookupError : std::logic_error
    {
        using std::logic_error::logic_error;
    };

    struct ArgumentError : std::logic_error
    {
        using std::logic_error::logic_error;
    };
}

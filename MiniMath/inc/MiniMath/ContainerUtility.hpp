#pragma once

#include <algorithm>

namespace mm
{
    constexpr bool contains(auto const& range, auto const& element)
    {
        return std::ranges::find(range, element) != std::ranges::end(range);        
    }
}

#pragma once

#include <istream>

namespace mm
{
    inline void skipBom(std::istream& stream)
    {
        auto a = stream.get();
        auto b = stream.get();
        auto c = stream.get();

        if (a != 0xEF || b != 0xBB || c != 0xBF)
            stream.seekg(0);
    }
}
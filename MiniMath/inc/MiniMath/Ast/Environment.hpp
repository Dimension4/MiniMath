#pragma once

#include <map>
#include <string>

namespace mm
{
    class Expr;

    namespace ast
    {
        using Environment = std::map<std::string, Expr>;
    }
}

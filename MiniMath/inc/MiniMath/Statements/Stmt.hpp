#pragma once

#include "../Expressions/Expr.hpp"

#include <variant>
#include <string>

namespace mm
{
    namespace stmt
    {
        //
        // register new statement types here
        //

        struct LetStmt
        {
            std::string name;
            Expr value;
        };

        struct ImportStmt
        {
            std::string target;
        };

        using StmtBase = std::variant<LetStmt, ImportStmt>;
    }

    class Stmt : public stmt::StmtBase
    {
    public:
        using stmt::StmtBase::StmtBase;
        using stmt::StmtBase::operator=;
    };
}

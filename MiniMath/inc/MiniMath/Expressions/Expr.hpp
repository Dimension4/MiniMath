#pragma once

#include "LeafExpressions.hpp"
#include "Recursive.hpp"

#include <variant>

namespace mm
{
    namespace expressions
    {
        //
        // forward declare new RECURSIVE expression types here
        //

        struct BinaryExpr;
        struct CallExpr;
        struct Closure;
        struct LetExpr;

        //
        // register new expression types here
        //

        using ExprBase = std::variant<
            Recursive<BinaryExpr>,
            Recursive<CallExpr>,
            Recursive<Closure>,
            Recursive<LetExpr>,
            ConstantExpr,
            NameExpr
        >;
    }

    class Expr : public expressions::ExprBase
    {
    public:
        using expressions::ExprBase::ExprBase;
        using expressions::ExprBase::operator=;
    };

}

#pragma once

#include "LeafExpressions.hpp"
#include "Recursive.hpp"

#include <variant>

namespace mm
{
    namespace expr
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

    class Expr : public expr::ExprBase
    {
    public:
        using Base = expr::ExprBase;

        using expr::ExprBase::ExprBase;
        using expr::ExprBase::operator=;
    };

}

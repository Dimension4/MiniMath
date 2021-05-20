﻿#pragma once

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
        struct FunctionExpr;
        struct InverseExpr;
        struct IfExpr;

        //
        // register new expression types here
        //

        using ExprBase = std::variant<
            NumberExpr,
            BoolExpr,
            NameExpr,
            Recursive<BinaryExpr>,
            Recursive<FunctionExpr>,
            Recursive<CallExpr>,
            Recursive<Closure>,
            Recursive<LetExpr>,
            Recursive<InverseExpr>,
            Recursive<IfExpr>
        >;
    }

    class Expr : public expr::ExprBase
    {
    public:
        using expr::ExprBase::ExprBase;
        using expr::ExprBase::operator=;
    };

}

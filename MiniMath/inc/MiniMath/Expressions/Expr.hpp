#pragma once

#include "LeafExpressions.hpp"
#include "Recursive.hpp"

#include <type_traits>
#include <variant>
#include <concepts>

namespace mm
{
    namespace expressions
    {
        //
        // forward declare new RECURSIVE expression types here
        //

        struct BinaryExpr;
        struct CallExpr;

        //
        // register new expression types here
        //

        using ExprBase = std::variant<
            Recursive<BinaryExpr>,
            Recursive<CallExpr>,
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

    template <typename T>
    concept RecursiveExpr = requires(expressions::Recursive<T> rec) { Expr{rec}; };

    template <typename T>
    concept LeafExpr = !RecursiveExpr<T> && requires(T t) { Expr{t}; };


    template <RecursiveExpr T, typename ...Args> requires(std::constructible_from<T, Args...>)
    [[nodiscard]] Expr makeExpr(Args&&... args)
    {
        return Expr(expressions::Recursive<T>(std::forward<Args>(args)...));
    }

    template <LeafExpr T, typename ...Args> requires(std::constructible_from<T, Args...>)
    [[nodiscard]] Expr makeExpr(Args&&... args)
    {
        return Expr(T(std::forward<Args>(args)...));
    }
}

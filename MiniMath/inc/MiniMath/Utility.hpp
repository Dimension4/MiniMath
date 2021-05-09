#pragma once

#include "Expressions/Expr.hpp"

#include <optional>
#include <type_traits>
#include <utility>
#include <functional>
#include <variant>

namespace mm
{
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

    template <typename T> requires(RecursiveExpr<T> || LeafExpr<T>)
    [[nodiscard]] constexpr bool isExprType(Expr const& expr) noexcept
    {
        if constexpr (RecursiveExpr<T>)
            return std::holds_alternative<expressions::Recursive<T>>(expr);
        else
            return std::holds_alternative<T>(expr);
    }

    template <InstanceOf<expressions::Recursive> T>
    [[nodiscard]] constexpr decltype(auto) unrec(T&& x) noexcept
    {
        return *std::forward<T>(x);
    }

    template <typename T>
    [[nodiscard]] constexpr decltype(auto) unrec(T&& x) noexcept
    {
        return std::forward<T>(x);
    }

    template <typename T> requires(RecursiveExpr<T> || LeafExpr<T>)
    [[nodiscard]] constexpr T const* tryGetExpr(Expr const& expr) noexcept
    {
        auto ret = [](auto* ptr) { return ptr ? &unrec(*ptr) : nullptr; };

        if constexpr (RecursiveExpr<T>)
            return ret(std::get_if<expressions::Recursive<T>>(&expr));
        else
            return ret(std::get_if<T>(&expr));
    }

    template <typename T> requires(RecursiveExpr<T> || LeafExpr<T>)
    [[nodiscard]] constexpr std::optional<T> tryGetExpr(Expr&& expr) noexcept
    {
        auto ret = [](auto* ptr) { return ptr ? std::optional(unrec(*ptr)) : std::nullopt; };

        if constexpr (RecursiveExpr<T>)
            return ret(std::get_if<expressions::Recursive<T>>(&expr));
        else
            return ret(std::get_if<T>(&expr));
    }

    namespace details
    {
        template <typename Visitor, typename Variant, typename ...Args>
        constexpr decltype(auto) visit(Visitor&& visitor, Variant&& variant, Args&& ...args)
        {
            return std::visit([&]<typename T>(T&& var) mutable
            {
                return std::forward<Visitor>(visitor)(unrec(std::forward<T>(var)), std::forward<Args>(args)...);
            }, std::forward<Variant>(variant));
        }
    }

    template <typename Visitor, typename ...Args>
    constexpr decltype(auto) visit(Visitor&& visitor, expressions::ExprBase&& expr, Args&& ...args)
    {
        return details::visit(std::forward<Visitor>(visitor), std::move(expr), std::forward<Args>(args)...);
    }

    template <typename Visitor, typename ...Args>
    constexpr decltype(auto) visit(Visitor&& visitor, expressions::ExprBase& expr, Args&& ...args)
    {
        return details::visit(std::forward<Visitor>(visitor), expr, std::forward<Args>(args)...);
    }

    template <typename Visitor, typename ...Args>
    constexpr decltype(auto) visit(Visitor&& visitor, expressions::ExprBase const& expr, Args&& ...args)
    {
        return details::visit(std::forward<Visitor>(visitor), expr, std::forward<Args>(args)...);
    }
}

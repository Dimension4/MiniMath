#pragma once

#include "../Expressions.hpp"

#include <fmt/format.h>

#include <ranges>

namespace mm::ast
{
    struct Printer
    {
        auto operator()(auto outIt, expr::BinaryExpr const& expr) const
        {
            return fmt::format_to(outIt, "({} {} {})", expr.left, enumDisplay(expr.operation), expr.right);
        }

        auto operator()(auto outIt, expr::CallExpr const& expr) const
        {
            return fmt::format_to(outIt, "{}({})", expr.target, fmt::join(expr.args, ", "));
        }

        auto operator()(auto outIt, expr::NameExpr const& expr) const
        {
            return fmt::format_to(outIt, "{}", expr.name);
        }

        auto operator()(auto outIt, expr::ConstantExpr const& expr) const
        {
            return fmt::format_to(outIt, "{}", expr.value);
        }

        auto operator()(auto outIt, expr::Closure const& expr) const
        {
            auto env = expr.environment | std::views::transform([](auto&& p) { return p.first; });
            return fmt::format_to(outIt, "[{}] {} -> {}", fmt::join(env, ", "), fmt::join(expr.paramNames, " "),
                                  expr.body);
        }

        auto operator()(auto outIt, expr::LetExpr const& expr) const
        {
            return fmt::format_to(outIt, "let {} = {} in {}", expr.name, expr.value, expr.body);
        }

        auto operator()(auto outIt, expr::FunctionExpr const& expr) const
        {
            return fmt::format_to(outIt, "fn {} -> {}", fmt::join(expr.paramNames, " "), expr.body);
        }
    };
}

template <>
struct fmt::formatter<mm::Expr>
{
    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(mm::Expr const& expr, FormatContext& ctx)
    {
        return std::visit([&]<typename T>(T&& x)
        {
            return mm::ast::Printer{}(ctx.out(), mm::unrec(std::forward<T>(x)));
        }, static_cast<mm::expr::ExprBase const&>(expr));
    }
};

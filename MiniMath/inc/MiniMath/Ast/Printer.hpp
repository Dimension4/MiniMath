#pragma once

#include "../Expressions.hpp"

#include <fmt/format.h>

namespace mm::ast
{
    struct Printer
    {
        auto operator()(auto outIt, expressions::BinaryExpr const& expr) const
        {
            return fmt::format_to(outIt, "({} {} {})", expr.left, enumDisplay(expr.operation), expr.right);
        }

        auto operator()(auto outIt, expressions::CallExpr const& expr) const
        {
            return fmt::format_to(outIt, "{}({})", expr.target, fmt::join(expr.args, ", "));
        }

        auto operator()(auto outIt, expressions::NameExpr const& expr) const
        {
            return fmt::format_to(outIt, "{}", expr.name);
        }

        auto operator()(auto outIt, expressions::ConstantExpr const& expr) const
        {
            return fmt::format_to(outIt, "{}", expr.value);
        }

        auto operator()(auto outIt, expressions::Closure const& expr) const
        {
            return fmt::format_to(outIt, "{}", expr.body);
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
        }, static_cast<mm::expressions::ExprBase const&>(expr));
    }
};

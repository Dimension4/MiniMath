#pragma once

#include "../Expressions.hpp"

#include <fmt/format.h>

#include <ranges>

namespace mm::ast
{
    struct Printer : StrictVisitor
    {
        auto operator()(expr::BinaryExpr const& expr, auto outIt) const
        {
            return fmt::format_to(outIt, "({} {} {})", expr.left, expr.operation, expr.right);
        }

        auto operator()(expr::CallExpr const& expr, auto outIt) const
        {
            return fmt::format_to(outIt, "{}({})", expr.target, fmt::join(expr.args, ", "));
        }

        auto operator()(expr::NameExpr const& expr, auto outIt) const
        {
            return fmt::format_to(outIt, "{}", expr.name);
        }

        auto operator()(expr::NumberExpr expr, auto outIt) const
        {
            return fmt::format_to(outIt, "{}", expr);
        }

        auto operator()(expr::BoolExpr expr, auto outIt) const
        {
            return fmt::format_to(outIt, "{}", expr);
        }

        auto operator()(expr::Closure const& expr, auto outIt) const
        {
            auto env = expr.environment | std::views::transform([](auto&& p) { return p.first; });
            return fmt::format_to(outIt, "[{}] {} -> {}",
                fmt::join(env, ", "), fmt::join(expr.paramNames, " "), expr.body);
        }

        auto operator()(expr::LetExpr const& expr, auto outIt) const
        {
            return fmt::format_to(outIt, "let {} = {} in {}", expr.name, expr.value, expr.body);
        }

        auto operator()(expr::FunctionExpr const& expr, auto outIt) const
        {
            return fmt::format_to(outIt, "fn {} -> {}", fmt::join(expr.paramNames, " "), expr.body);
        }

        auto operator()(stmt::LetStmt const& stmt, auto outIt) const
        {
            return fmt::format_to(outIt, "let {} = {}", stmt.name, stmt.value);
        }

        auto operator()(stmt::ImportStmt const& stmt, auto outIt) const
        {
            return fmt::format_to(outIt, "import ", stmt.target);
        }

        auto operator()(expr::InverseExpr const& expr, auto outIt) const
        {
            return fmt::format_to(outIt, "-{}", expr.body);
        }

        auto operator()(expr::ops::Add, auto outIt) const
        {
            return fmt::format_to(outIt, "+");
        }

        auto operator()(expr::ops::Subtract, auto outIt) const
        {
            return fmt::format_to(outIt, "-");
        }

        auto operator()(expr::ops::Multiply, auto outIt) const
        {
            return fmt::format_to(outIt, "*");
        }

        auto operator()(expr::ops::Divide, auto outIt) const
        {
            return fmt::format_to(outIt, "/");
        }
    };
}

template <typename T> requires(std::same_as<T, mm::Expr> || std::same_as<T, mm::expr::BinaryOperation> || std::same_as<T, mm::Stmt>)
struct fmt::formatter<T>
{
    constexpr auto parse(format_parse_context& ctx) const
    {
        return ctx.begin();
    }

    auto format(auto const& expr, auto& ctx) const
    {
        return mm::visit(mm::ast::Printer{}, expr, ctx.out());
    }
};

template <typename T> requires((mm::ExprType<T> || mm::VariantMember<T, mm::expr::BinaryOperation>) && !std::is_fundamental_v<T>)
struct fmt::formatter<T>
{
    constexpr auto parse(format_parse_context& ctx) const
    {
        return ctx.begin();
    }

    auto format(auto const& expr, auto& ctx) const
    {
        return mm::ast::Printer{}(expr, ctx.out());
    }
};

#pragma once

#include "MiniMath/Utility.hpp"

#include <fmt/format.h>

#include <map>
#include <string>
#include <functional>
#include <filesystem>

namespace mm
{
    class Expr;

    namespace ast
    {
        class Environment
        {
        public:
            using ConstIterator = std::map<std::string, Expr, std::less<>>::const_iterator;

            [[nodiscard]]
            Environment() = default;

            [[nodiscard]]
            Environment(std::initializer_list<std::pair<const std::string, Expr>> bindings);

            [[nodiscard]]
            Expr const* tryGet(std::string_view name) const noexcept;

            template <ExprType T>
            [[nodiscard]]
            T const* tryGetAs(std::string_view name) const noexcept
            {
                auto expr = tryGet(name);
                return expr ? &tryGetExpr<T>(*expr) : nullptr;
            }

            [[nodiscard]]
            Environment with(std::string name, Expr value) const;

            [[nodiscard]]
            std::filesystem::path const& getDir() const noexcept;
            void setDir(std::filesystem::path path);

            void add(std::string name, Expr value);
            void merge(Environment&& other);

            [[nodiscard]]
            ConstIterator begin() const noexcept;
            [[nodiscard]]
            ConstIterator end() const noexcept;

            friend bool operator==(Environment const&, Environment const&) = default;

        private:
            std::map<std::string, Expr, std::less<>> bindings_;
            std::filesystem::path dir_ = ".";
        };
    }
}


template <>
struct fmt::formatter<mm::ast::Environment>
{
    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(mm::ast::Environment const& env, FormatContext& ctx)
    {
        fmt::format_to(ctx.out(), "{{\n");
        for (auto const& [name, val] : env)
            fmt::format_to(ctx.out(), "    {} = {}\n", name, val);
        fmt::format_to(ctx.out(), "}}");
        return ctx.out();
    }
};

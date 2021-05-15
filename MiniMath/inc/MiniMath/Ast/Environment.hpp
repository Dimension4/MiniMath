﻿#pragma once

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
            [[nodiscard]]
            Expr const* tryGet(std::string_view name) const noexcept
            {
                if (auto it = symbols_.find(name); it != symbols_.end())
                    return &it->second;

                return nullptr;
            }

            template <ExprType T>
            [[nodiscard]]
            T const* tryGetAs(std::string_view name) const noexcept
            {
                auto expr = tryGet(name);
                return expr ? &tryGetExpr<T>(*expr) : nullptr;
            }

            [[nodiscard]]
            Environment with(std::string name, Expr value) const
            {
                auto copy = *this;
                copy.add(move(name), std::move(value));
                return copy;
            }

            void setDir(std::filesystem::path path)
            {
                dir_ = std::move(path);
            }

            [[nodiscard]]
            std::filesystem::path const& getDir() const noexcept
            {
                return dir_;
            }

            void add(std::string name, Expr value)
            {
                symbols_.insert_or_assign(std::move(name), std::move(value));
            }

            void merge(Environment&& other)
            {
                for (auto& [name, val] : other.symbols_)
                    symbols_.insert_or_assign(name, std::move(val));
            }

            auto begin() const noexcept
            {
                return symbols_.cbegin();
            }

            auto end() const noexcept
            {
                return symbols_.cend();
            }

            friend bool operator==(Environment const&, Environment const&) = default;

        private:
            std::map<std::string, Expr, std::less<>> symbols_;
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

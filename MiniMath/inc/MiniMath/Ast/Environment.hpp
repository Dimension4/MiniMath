﻿#pragma once

#include "MiniMath/Utility.hpp"

#include <map>
#include <string>
#include <functional>

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

            void add(std::string name, Expr value)
            {
                symbols_.insert_or_assign(std::move(name), std::move(value));
            }

            void merge(Environment&& other)
            {
                for (auto& [name, val] : other.symbols_)
                    symbols_.insert_or_assign(move(name), std::move(val));
            }

            auto begin() const noexcept
            {
                return symbols_.cbegin();
            }

            auto end() const noexcept
            {
                return symbols_.cend();
            }

        private:
            std::map<std::string, Expr, std::less<>> symbols_;
        };
    }
}

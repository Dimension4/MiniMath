#pragma once

#include "Expressions.hpp"

#include <utility>
#include <functional>

namespace mm
{
    template <typename Visitor>
    class RecursiveVisitor
    {
    public:
        explicit constexpr RecursiveVisitor(Visitor visitor = {}) :
            visitor_(std::move(visitor)) { }

        template <typename T, typename ...Args>
        constexpr decltype(auto) operator()(T&& variant, Args&& ...args) const
        {
            return std::visit([&]<typename U>(U && item)
            {
                return std::invoke(visitor_, *this, expressions::unrec(std::forward<U>(item)), std::forward<Args>(args)...);
            }, std::forward<T>(variant));
        }

    private:
        Visitor visitor_;
    };
}

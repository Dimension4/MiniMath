#pragma once

#include <fmt/format.h>

#include <memory>
#include <string>
#include <type_traits>

namespace mm
{
    namespace expressions
    {
        class Expression
        {
        public:
            virtual ~Expression() = default;
            [[nodiscard]] virtual std::string toString() const = 0;
        };

        using ExpressionPtr = std::unique_ptr<Expression>;
    }

    namespace concepts
    {
        template <typename T>
        concept Expression = std::is_base_of_v<expressions::Expression, std::remove_cvref_t<T>>;

        template <typename T>
        concept ExpressionPtr = requires (T x)
        {
            { *x } -> Expression;
            { *(x.operator->()) } -> Expression;
        };
    }
}

template <mm::concepts::Expression T>
struct fmt::formatter<T> : formatter<std::string>
{
    template <typename FormatContext>
    auto format(const T& exp, FormatContext& ctx)
    {
        return formatter<string_view>::format(exp.toString(), ctx);
    }
};

template <mm::concepts::ExpressionPtr T>
struct fmt::formatter<T> : formatter<std::string>
{
    template <typename FormatContext>
    auto format(const T& exp, FormatContext& ctx)
    {
        return formatter<string_view>::format(exp->toString(), ctx);
    }
};

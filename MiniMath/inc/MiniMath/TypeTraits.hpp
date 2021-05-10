#pragma once

#include <type_traits>

namespace mm
{
    namespace details
    {
        template <typename, template <typename, typename...> typename>
        struct IsInstance : std::false_type {};

        template <typename...Ts, template <typename, typename...> typename U>
        struct IsInstance<U<Ts...>, U> : std::true_type {};

        template <typename Test, typename Variant>
        struct CountVariantMembers;

        template <typename Test, template <typename ...> typename Variant, typename ...Args>
        struct CountVariantMembers<Test, Variant<Args...>>
        {
            static constexpr std::size_t Count = (std::same_as<Test, Args> + ...);
        };
    }

    template <typename T, template <typename, typename...> typename Template>
    concept InstanceOf = details::IsInstance<std::remove_cvref_t<T>, Template>::value;

    template <typename T, typename Variant>
    concept VariantMember = details::CountVariantMembers<T, Variant>::Count > 0;
}

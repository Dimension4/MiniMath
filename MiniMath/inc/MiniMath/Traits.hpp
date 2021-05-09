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
    }

    template <typename T, template <typename, typename...> typename Template>
    concept InstanceOf = details::IsInstance<std::remove_cvref_t<T>, Template>::value;
}

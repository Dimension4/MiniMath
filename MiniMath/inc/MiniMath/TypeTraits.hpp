#pragma once

#include <type_traits>

namespace mm
{
    namespace details
    {
        template <typename, template <typename...> typename>
        struct IsInstance : std::false_type {};

        template <typename...Ts, template <typename...> typename U>
        struct IsInstance<U<Ts...>, U> : std::true_type {};

        template <typename Test, typename Variant>
        struct CountVariantMembers;

        template <typename Test, template <typename ...> typename Variant, typename ...Args>
        struct CountVariantMembers<Test, Variant<Args...>>
        {
            static constexpr std::size_t Count = (std::same_as<Test, Args> + ...);
        };
    }

    template <typename T, template <typename...> typename Template>
    concept InstanceOf = details::IsInstance<std::remove_cvref_t<T>, Template>::value;

    template <typename Target, typename Source> requires(std::same_as<std::remove_cvref_t<Target>, Target>)
    using CopyQualifiers = std::conditional_t<
        std::is_lvalue_reference_v<Source>,
        std::add_lvalue_reference_t<std::conditional_t<
            std::is_const_v<std::remove_reference_t<Source>>,
            std::add_const_t<Target>,
            Target>>,
        std::add_rvalue_reference_t<Target>>;

    namespace details
    {
        template <template <typename...> typename T, typename... Args>
        auto variantBase(T<Args...>) -> T<Args...>;
    }

    template <typename T>
    using BaseVariant = decltype(details::variantBase(std::declval<T>()));

    template <typename T, typename Variant>
    concept VariantMember = details::CountVariantMembers<T, BaseVariant<Variant>>::Count > 0;

    template <typename T>
    concept Visitable = InstanceOf<BaseVariant<T>, std::variant>;
}

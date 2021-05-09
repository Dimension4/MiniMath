#pragma once

#include "../Traits.hpp"

#include <utility>

namespace mm::expressions
{
    template <typename T>
    class Recursive
    {
    public:
        template <typename ...Args>
        [[nodiscard]] explicit constexpr Recursive(Args&& ...args);
        [[nodiscard]] explicit constexpr Recursive(T val);
        ~Recursive() noexcept;

        [[nodiscard]] constexpr Recursive(Recursive const& other);

        constexpr Recursive& operator=(Recursive const& other)
        {
            if (this != &other)
                *ptr_ = *other.ptr_;

            return *this;
        }

        [[nodiscard]] constexpr Recursive(Recursive&& other) noexcept :
            ptr_(std::exchange(other.ptr_, nullptr)) {}

        constexpr Recursive& operator=(Recursive&& other) noexcept
        {
            if (this != &other)
                ptr_ = std::exchange(other.ptr_, nullptr);

            return *this;
        }

        [[nodiscard]] constexpr T const& operator*() const noexcept { return *ptr_; }
        [[nodiscard]] constexpr T& operator*() noexcept { return *ptr_; }

        [[nodiscard]] constexpr T const* operator->() const noexcept { return ptr_; }
        [[nodiscard]] constexpr T* operator->() noexcept { return ptr_; }

    private:
        T* ptr_;
    };

    template <InstanceOf<Recursive> T>
    [[nodiscard]] constexpr decltype(auto) unrec(T&& x) noexcept
    {
        return *std::forward<T>(x);
    }

    template <typename T>
    [[nodiscard]] constexpr decltype(auto) unrec(T&& x) noexcept
    {
        return std::forward<T>(x);
    }
}
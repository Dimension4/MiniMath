#pragma once

#include <utility>

namespace mm::expr
{
    template <typename T>
    class Recursive;

    template <typename T>
    bool operator==(Recursive<T> const&, Recursive<T> const&);

    template <typename T>
    class Recursive
    {
    public:
        using ValueType = T;

        template <typename ...Args> requires(sizeof...(Args) > 0)
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
            ptr_(std::exchange(other.ptr_, nullptr))
        {
        }

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

        friend bool operator==<>(Recursive const&, Recursive const&);

    private:
        T* ptr_;
    };
}

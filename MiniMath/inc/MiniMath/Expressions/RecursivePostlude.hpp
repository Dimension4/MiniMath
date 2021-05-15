#pragma once

#include "Recursive.hpp"

#include <concepts>

namespace mm::expr
{
    template <typename T>
    template <typename ...Args> requires(sizeof...(Args) > 0)
    constexpr Recursive<T>::Recursive(Args&& ...args) :
        ptr_(new T(std::forward<Args>(args)...))
    {
    }

    template <typename T>
    constexpr Recursive<T>::Recursive(T val) :
        ptr_(new T(std::move(val)))
    {
    }

    template <typename T>
    constexpr Recursive<T>::Recursive(Recursive const& other) :
        ptr_(new T(*other.ptr_))
    {
    }

    template <typename T>
    Recursive<T>::~Recursive() noexcept
    {
        static_assert(sizeof(T), "Cannot delete incomplete type");
        delete ptr_;
    }

    template <typename T>
    bool operator==(Recursive<T> const& lhs, Recursive<T> const& rhs)
    {
        static_assert(std::equality_comparable<T>);

        if ((lhs.ptr_ == nullptr) ^ (rhs.ptr_ == nullptr))
            return false;

        if (lhs.ptr_ == rhs.ptr_)
            return true;

        return *lhs.ptr_ == *rhs.ptr_;
    }
}

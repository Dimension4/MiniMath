#pragma once

#include "Recursive.hpp"

namespace mm::expressions
{
    template <typename T>
    template <typename ...Args>
    constexpr Recursive<T>::Recursive(Args&& ...args) :
        ptr_(new T(std::forward<Args>(args)...)) {}

    template <typename T>
    constexpr Recursive<T>::Recursive(T val) :
        ptr_(new T(std::move(val))) {}

    template <typename T>
    constexpr Recursive<T>::Recursive(Recursive const& other) :
        ptr_(new T(*other.ptr_)) {}

    template <typename T>
    Recursive<T>::~Recursive() noexcept
    {
        static_assert(sizeof(T), "Cannot delete incomplete type");
        delete ptr_;
    }
}

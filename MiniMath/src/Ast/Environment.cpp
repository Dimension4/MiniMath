#include "MiniMath/Ast/Environment.hpp"

#include "MiniMath/Expressions.hpp"

namespace mm::ast
{
    Environment::Environment(std::initializer_list<std::pair<const std::string, Expr>> bindings) :
        bindings_(bindings)
    {
    }


    [[nodiscard]]
    Expr const* Environment::tryGet(std::string_view name) const noexcept
    {
        if (auto it = bindings_.find(name); it != bindings_.end())
            return &it->second;

        return nullptr;
    }

    [[nodiscard]]
    Environment Environment::with(std::string name, Expr value) const
    {
        auto copy = *this;
        copy.add(move(name), std::move(value));
        return copy;
    }

    void Environment::setDir(std::filesystem::path path)
    {
        dir_ = std::move(path);
    }

    [[nodiscard]]
    std::filesystem::path const& Environment::getDir() const noexcept
    {
        return dir_;
    }

    void Environment::add(std::string name, Expr value)
    {
        bindings_.insert_or_assign(std::move(name), std::move(value));
    }

    void Environment::merge(Environment&& other)
    {
        for (auto& [name, val] : other.bindings_)
            bindings_.insert_or_assign(name, std::move(val));
    }

    Environment::ConstIterator Environment::begin() const noexcept
    {
        return bindings_.cbegin();
    }

    Environment::ConstIterator Environment::end() const noexcept
    {
        return bindings_.cend();
    }
}

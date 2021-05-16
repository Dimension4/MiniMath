#pragma once

#include "MiniMath/Expressions.hpp"

#include <cstddef>
#include <ostream>

namespace expr
{
    inline mm::Expr operator""_name(char const* val, std::size_t)
    {
        return mm::expr::NameExpr{ val };
    }

    inline mm::Expr operator""_const(long double val)
    {
        return mm::expr::ConstantExpr{ double(val) };
    }

    inline mm::Expr operator""_const(unsigned long long val)
    {
        return mm::expr::ConstantExpr{ double(val) };
    }

    inline mm::Expr binexpr(mm::expr::BinaryOperation op, mm::Expr left, mm::Expr right)
    {
        return mm::makeExpr(mm::expr::BinaryExpr{
            .operation = op,
            .left = std::move(left),
            .right = std::move(right)
        });
    }

    inline mm::Expr fnexpr(std::vector<std::string> params, mm::Expr body)
    {
        return mm::makeExpr(mm::expr::FunctionExpr{ move(params), std::move(body) });
    }

    inline mm::Expr closure(std::vector<std::string> params, mm::Expr body, mm::ast::Environment env = {})
    {
        return mm::makeExpr(mm::expr::Closure{
            std::move(env),
            move(params),
            std::move(body)
        });
    }

    inline mm::Expr callexpr(mm::Expr target, std::vector<mm::Expr> args)
    {
        return mm::makeExpr(mm::expr::CallExpr{ std::move(target), std::move(args) });
    }

    inline mm::Expr operator+(mm::Expr const& lhs, mm::Expr const& rhs)
    {
        return binexpr(mm::expr::BinaryOperation::Add, lhs, rhs);
    }

    inline mm::Expr operator-(mm::Expr const& lhs, mm::Expr const& rhs)
    {
        return binexpr(mm::expr::BinaryOperation::Subtract, lhs, rhs);
    }

    inline mm::Expr operator*(mm::Expr const& lhs, mm::Expr const& rhs)
    {
        return binexpr(mm::expr::BinaryOperation::Multiply, lhs, rhs);
    }

    inline mm::Expr operator/(mm::Expr const& lhs, mm::Expr const& rhs)
    {
        return binexpr(mm::expr::BinaryOperation::Divide, lhs, rhs);
    }

    struct LetValBuilder
    {
        std::string name;
        mm::Expr value;

        mm::Expr operator>>(mm::Expr&& body) &&
        {
            return mm::makeExpr(mm::expr::LetExpr{
                move(name), std::move(value), std::move(body)
            });
        }
    };

    struct LetNameBuilder
    {
        std::string name;

        LetValBuilder operator=(mm::Expr val) &&
        {
            return LetValBuilder{ move(name), std::move(val) };
        }
    };

    inline LetNameBuilder operator""_let(char const* val, std::size_t)
    {
        return LetNameBuilder{ .name = val };
    }
}

namespace mm
{
    inline std::ostream& operator<<(std::ostream& os, Expr const& expr)
    {
        return os << fmt::format("{}", expr);
    }
}

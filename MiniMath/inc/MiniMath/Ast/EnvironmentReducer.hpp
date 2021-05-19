#pragma once

#include "Environment.hpp"
#include "../Expressions.hpp"

namespace mm::ast
{
    struct EnvironmentReducer : StrictVisitor
    {
        void operator()(expr::BinaryExpr const& expr, std::vector<std::string> const& params, Environment const& inEnv, Environment& outEnv) const;
        void operator()(expr::CallExpr const& expr, std::vector<std::string> const& params, Environment const& inEnv, Environment& outEnv) const;
        void operator()(expr::NameExpr const& expr, std::vector<std::string> const& params, Environment const& inEnv, Environment& outEnv) const;
        void operator()(expr::NumberExpr expr, std::vector<std::string> const& params, Environment const& inEnv, Environment& outEnv) const;
        void operator()(expr::BoolExpr expr, std::vector<std::string> const& params, Environment const& inEnv, Environment& outEnv) const;
        void operator()(expr::Closure const& expr, std::vector<std::string> const& params, Environment const& inEnv, Environment& outEnv) const;
        void operator()(expr::LetExpr const& expr, std::vector<std::string> const& params, Environment const& inEnv, Environment& outEnv) const;
        void operator()(expr::FunctionExpr const& expr, std::vector<std::string> const& params, Environment const& inEnv, Environment& outEnv) const;
        void operator()(expr::InverseExpr const& expr, std::vector<std::string> const& params, Environment const& inEnv, Environment& outEnv) const;
    };
}
#include "MiniMath/Ast/EnvironmentReducer.hpp"
#include "MiniMath/Ast/Errors.hpp"
#include "MiniMath/ContainerUtility.hpp"

#include <fmt/format.h>

namespace mm::ast
{
    using namespace expr;

    void EnvironmentReducer::operator()(BinaryExpr const& expr, std::vector<std::string> const& params,
                                        Environment const& inEnv, Environment& outEnv) const
    {
        visit(*this, expr.left, params, inEnv, outEnv);
        visit(*this, expr.right, params, inEnv, outEnv);
    }

    void EnvironmentReducer::operator()(CallExpr const& expr, std::vector<std::string> const& params,
                                        Environment const& inEnv, Environment& outEnv) const
    {
        visit(*this, expr.target, params, inEnv, outEnv);
        for (auto&& arg : expr.args)
            visit(*this, arg, params, inEnv, outEnv);
    }

    void EnvironmentReducer::operator()(NameExpr const& expr, std::vector<std::string> const& params,
                                        Environment const& inEnv, Environment& outEnv) const
    {
        if (contains(params, expr.name))
            return;

        if (auto var = inEnv.tryGet(expr.name))
            outEnv.add(expr.name, *var);
        else
            throw LookupError(fmt::format("Unknown identifier '{}'", expr.name));
    }

    void EnvironmentReducer::operator()(ConstantExpr const& expr, std::vector<std::string> const& params,
                                        Environment const& inEnv, Environment& outEnv) const
    {
    }

    void EnvironmentReducer::operator()(Closure const& expr, std::vector<std::string> const& params,
                                        Environment const& inEnv, Environment& outEnv) const
    {
    }

    void EnvironmentReducer::operator()(LetExpr const& expr, std::vector<std::string> const& params,
                                        Environment const& inEnv, Environment& outEnv) const
    {
        auto copy = params;
        copy.push_back(expr.name);
        visit(*this, expr.value, params, inEnv, outEnv);
        visit(*this, expr.body, copy, inEnv, outEnv);
    }

    void EnvironmentReducer::operator()(FunctionExpr const& expr, std::vector<std::string> const& params,
                                        Environment const& inEnv, Environment& outEnv) const
    {
        auto copy = params;
        for (auto&& param : expr.paramNames)
            copy.push_back(param);

        visit(*this, expr.body, copy, inEnv, outEnv);
    }
}

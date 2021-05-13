#pragma once

#include "Environment.hpp"
#include "../Expressions.hpp"
#include "../Statements/Stmt.hpp"


namespace mm::ast
{
    struct ExprEvaluator
    {
        Expr operator()(expr::BinaryExpr const& expr, Environment const& env) const;
        Expr operator()(expr::CallExpr const& expr, Environment const& env) const;
        Expr operator()(expr::NameExpr const& expr, Environment const& env) const;
        Expr operator()(expr::ConstantExpr const& expr, Environment const& env) const;
        Expr operator()(expr::Closure const& expr, Environment const& env) const;
        Expr operator()(expr::LetExpr const& expr, Environment const& env) const;
        Expr operator()(expr::FunctionExpr const& expr, Environment const& env) const;
    };

	struct StmtEvaluator
	{
		Environment operator()(stmt::LetStmt const& stmt, Environment const& env) const;
	};

    inline Expr evaluate(Expr const& expr, Environment const& env = {})
    {
        return visit(ExprEvaluator{}, expr, env);
    }
	
    inline Environment evaluate(Stmt const& expr, Environment const& env)
    {
        return visit(StmtEvaluator{}, expr, env);
    }
}

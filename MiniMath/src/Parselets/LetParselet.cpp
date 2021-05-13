#include "MiniMath/Parselets/LetParselet.hpp"

#include "MiniMath/Expressions.hpp"
#include "MiniMath/Statements/Stmt.hpp"
#include "MiniMath/PrattParser.hpp"


namespace mm::parselets
{
	using namespace expr;
	using namespace stmt;

	Expr LetParselet::parse(PrattParser& parser, const Token& token) const
	{
		auto name = parser.consume(TokenType::Identifier);
		parser.consume(TokenType::Equals);
		auto value = parser.parseExpression();
	    std::ignore = parser.match(TokenType::In);
		auto body = parser.parseExpression();

		return makeExpr(LetExpr{ .name = move(name.lexeme), .value = std::move(value), .body = std::move(body) });
	}

	Stmt LetStmtParselet::parse(PrattParser& parser, const Token& token) const
	{
		auto name = parser.consume(TokenType::Identifier);
		parser.consume(TokenType::Equals);
		auto value = parser.parseExpression();

		return LetStmt{ .name = move(name.lexeme), .value = move(value) };
	}
}

#include "MiniMath/Parselets/BinaryOpParselet.hpp"

#include "MiniMath/Expressions.hpp"
#include "MiniMath/PrattParser.hpp"

#include <fmt/format.h>

using namespace mm;
using namespace parselets;
using namespace expr;

static BinaryOperation tokenToOp(TokenType type)
{
    switch (type)
    {
    case TokenType::Plus: return ops::Add{};
    case TokenType::Minus: return ops::Subtract{};
    case TokenType::Asterisk: return ops::Multiply{};
    case TokenType::Slash: return ops::Divide{};
    case TokenType::And: return ops::And{};
    case TokenType::Or: return ops::Or{};
    case TokenType::Equals: return ops::Equals{};
    case TokenType::Less: return ops::Less{};
    case TokenType::LessEqual: return ops::LessThan{};
    case TokenType::Greater: return ops::Greater{};
    case TokenType::GreaterEqual: return ops::GreaterThan{};
    }

    throw ParseError(fmt::format("Invalid token type '{}' for binary expression", enumName(type)));
}


BinaryOpParselet::BinaryOpParselet(int precedence, Associativity associativity) :
    precedence_(precedence), associativity_(associativity) { }

Expr BinaryOpParselet::parse(PrattParser& parser, const Expr& left, const Token& token) const
{
    auto precedence = associativity_ == Associativity::Left ? precedence_ : precedence_ - 1;
    auto right = parser.parseExpression(precedence);

    return makeExpr(BinaryExpr{ .operation = tokenToOp(token.type), .left = left, .right = move(right) });
}

int BinaryOpParselet::getPrecedence() const
{
    return precedence_;
}

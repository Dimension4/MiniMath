#include "MiniMath/Parselets/BinaryOpParselet.hpp"

#include "MiniMath/Expressions/BinaryExpression.hpp"

using namespace mm;
using namespace parselets;
using namespace expressions;

static BinaryOperation tokenToOp(TokenType type)
{
    switch (type)
    {
    case TokenType::Plus: return BinaryOperation::Add;
    case TokenType::Minus: return BinaryOperation::Subtract;
    case TokenType::Asterisk: return BinaryOperation::Multiply;
    case TokenType::Slash: return BinaryOperation::Divide;
    }

    throw ParseError(fmt::format("Invalid token type '{}' for binary expression", enumName(type)));
}


BinaryOpParselet::BinaryOpParselet(int precedence, Associativity associativity) :
    precedence_(precedence), associativity_(associativity) { }

ExpressionPtr BinaryOpParselet::parse(PrattParser& parser, ExpressionPtr left, const Token& token) const
{
    auto precedence = associativity_ == Associativity::Left ? precedence_ : precedence_ - 1;
    auto right = parser.parseExpression(precedence);

    return std::make_unique<BinaryExpression>(tokenToOp(token.type), move(left), move(right));
}

int BinaryOpParselet::getPrecedence() const
{
    return precedence_;
}

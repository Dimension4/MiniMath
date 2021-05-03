﻿#include "MiniMath/MiniMathParser.hpp"

#include "MiniMath/Expressions/NameExpression.hpp"
#include "MiniMath/Parselets/ConstantParselet.hpp"
#include "MiniMath/Parselets/NameParselet.hpp"

using namespace mm;
using namespace expressions;
using namespace parselets;

MiniMathParser::MiniMathParser(Lexer& lexer) : PrattParser(lexer)
{
    registerParselet(TokenType::Identifier, std::make_unique<NameParselet>());
    registerParselet(TokenType::Number, std::make_unique<ConstantParselet>());

    infix(TokenType::Plus, OperatorPrecedence::Addition);
    infix(TokenType::Minus, OperatorPrecedence::Subtraction);
    infix(TokenType::Asterisk, OperatorPrecedence::Multiplication);
    infix(TokenType::Slash, OperatorPrecedence::Division);
}

void MiniMathParser::prefix(TokenType token, OperatorPrecedence precedence)
{
    // registerParselet(token, )
}
void MiniMathParser::postfix(TokenType token, OperatorPrecedence precedence)
{
    // registerParselet(token, )
}

void MiniMathParser::infix(TokenType token, OperatorPrecedence precedence, Associativity associativity)
{
    registerParselet(token, std::make_unique<BinaryOpParselet>(int(precedence), associativity));
}
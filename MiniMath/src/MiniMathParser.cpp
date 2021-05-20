﻿#include "MiniMath/MiniMathParser.hpp"

#include "MiniMath/Parselets/CallParselet.hpp"
#include "MiniMath/Parselets/ConstantParselet.hpp"
#include "MiniMath/Parselets/FunctionParselet.hpp"
#include "MiniMath/Parselets/GroupParselet.hpp"
#include "MiniMath/Parselets/LetParselet.hpp"
#include "MiniMath/Parselets/NameParselet.hpp"
#include "MiniMath/Parselets/ImportParselet.hpp"
#include "MiniMath/Parselets/PrefixOpParselet.hpp"


using namespace mm;
using namespace parselets;

MiniMathParser::MiniMathParser(std::function<Token()> tokenSource) :
    PrattParser(move(tokenSource))
{
    registerParselet(TokenType::Let, std::make_unique<LetStmtParselet>());
    registerParselet(TokenType::Import, std::make_unique<ImportParselet>());

    registerParselet(TokenType::Identifier, std::make_unique<NameParselet>());
    registerParselet(TokenType::Number, std::make_unique<ConstantParselet>());
    registerParselet(TokenType::True, std::make_unique<ConstantParselet>());
    registerParselet(TokenType::False, std::make_unique<ConstantParselet>());
    registerParselet(TokenType::LParen, std::make_unique<GroupParselet>());
    registerParselet(TokenType::LParen, std::make_unique<CallParselet>());
    registerParselet(TokenType::Let, std::make_unique<LetParselet>());
    registerParselet(TokenType::Fn, std::make_unique<FunctionParselet>());

    prefix(TokenType::Minus, OperatorPrecedence::Prefix);

    infix(TokenType::Plus, OperatorPrecedence::Addition);
    infix(TokenType::Minus, OperatorPrecedence::Subtraction);
    infix(TokenType::Asterisk, OperatorPrecedence::Multiplication);
    infix(TokenType::Slash, OperatorPrecedence::Division);
    infix(TokenType::And, OperatorPrecedence::And);
    infix(TokenType::Or, OperatorPrecedence::Or);
    infix(TokenType::Equals, OperatorPrecedence::Relational);
    infix(TokenType::Less, OperatorPrecedence::Relational);
    infix(TokenType::LessEqual, OperatorPrecedence::Relational);
    infix(TokenType::Greater, OperatorPrecedence::Relational);
    infix(TokenType::GreaterEqual, OperatorPrecedence::Relational);
}

void MiniMathParser::prefix(TokenType token, OperatorPrecedence precedence)
{
    registerParselet(token, std::make_unique<PrefixOpParselet>(int(precedence)));
}

void MiniMathParser::infix(TokenType token, OperatorPrecedence precedence, Associativity associativity)
{
    registerParselet(token, std::make_unique<BinaryOpParselet>(int(precedence), associativity));
}

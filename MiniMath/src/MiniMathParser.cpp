#include "MiniMath/MiniMathParser.hpp"

#include "MiniMath/Parselets/CallParselet.hpp"
#include "MiniMath/Parselets/ConstantParselet.hpp"
#include "MiniMath/Parselets/FunctionParselet.hpp"
#include "MiniMath/Parselets/GroupParselet.hpp"
#include "MiniMath/Parselets/LetParselet.hpp"
#include "MiniMath/Parselets/NameParselet.hpp"
#include "MiniMath/Parselets/ImportParselet.hpp"


using namespace mm;
using namespace parselets;

MiniMathParser::MiniMathParser(Lexer& lexer) : PrattParser(lexer)
{
    registerParselet(TokenType::Let, std::make_unique<LetStmtParselet>());
    registerParselet(TokenType::Import, std::make_unique<ImportParselet>());

    registerParselet(TokenType::Identifier, std::make_unique<NameParselet>());
    registerParselet(TokenType::Number, std::make_unique<ConstantParselet>());
    registerParselet(TokenType::LParen, std::make_unique<GroupParselet>());
    registerParselet(TokenType::LParen, std::make_unique<CallParselet>());
    registerParselet(TokenType::Let, std::make_unique<LetParselet>());
    registerParselet(TokenType::Fn, std::make_unique<FunctionParselet>());
    
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

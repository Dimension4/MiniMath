#include "MiniMath/PrattParser.hpp"

#include "MiniMath/Expressions.hpp"

#include <fmt/format.h>

using namespace mm;
using namespace parselets;
using namespace expr;

static auto tryGet(const auto& map, const auto& key)
{
    auto it = map.find(key);
    return it != map.end() ? it->second.get() : nullptr;
}

PrattParser::PrattParser(Lexer& lexer) : lexer_(lexer) {}

void PrattParser::registerParselet(TokenType type, std::unique_ptr<PrefixParselet> parselet)
{
    prefixParselets_[type] = move(parselet);
}

void PrattParser::registerParselet(TokenType type, std::unique_ptr<InfixParselet> parselet)
{
    infixParselets_[type] = move(parselet);
}

Expr PrattParser::parseExpression(int precedence)
{
    auto token = consume();
    auto prefix = tryGet(prefixParselets_, token.type);

    if (!prefix)
        throw ParseError(fmt::format("Unexpected token '{}'", enumDisplay(token.type)));

    auto left = prefix->parse(*this, token);

    while (precedence < getPrecedence())
    {
        token = consume();
        auto infix = tryGet(infixParselets_, token.type);
        left = infix->parse(*this, std::move(left), token);
    }

    return left;
}

bool PrattParser::match(TokenType expected)
{
    auto next = lookAhead();
    if (next.type != expected)
        return false;

    consume();
    return true;
}

Token PrattParser::consume(TokenType expected)
{
    auto next = lookAhead();
    if (next.type != expected)
    {
        throw ParseError(fmt::format(R"(Expected token "{}", but got "{}")", enumName(expected), enumName(next.type)));
    }

    return consume();
}

Token PrattParser::consume()
{
    lookAhead();
    auto next = tokens_[0];
    tokens_.erase(tokens_.begin());
    return next;
}

Token PrattParser::lookAhead(std::size_t distance)
{
    while (tokens_.size() < distance)
        tokens_.push_back(lexer_.nextToken());

    return tokens_[distance - 1];
}

int PrattParser::getPrecedence()
{
    auto parser = tryGet(infixParselets_, lookAhead().type);
    return parser ? parser->getPrecedence() : 0;
}

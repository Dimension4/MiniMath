#include "MiniMath/Parselets/ImportParselet.hpp"

#include "MiniMath/PrattParser.hpp"

#include <fmt/format.h>

namespace mm::parselets
{
    Stmt ImportParselet::parse(PrattParser& parser, const Token& token) const
    {
        std::vector<std::string> mods;
        mods.push_back(parser.consume(TokenType::Identifier).lexeme);

        while (parser.match(TokenType::Dot))
            mods.push_back(parser.consume(TokenType::Identifier).lexeme);

        return stmt::ImportStmt{ .target = fmt::to_string(fmt::join(mods, ".")) };
    }
}

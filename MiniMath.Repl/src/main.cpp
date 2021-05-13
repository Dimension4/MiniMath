#include "MiniMath/MiniMathParser.hpp"
#include "MiniMath/Expressions.hpp"
#include "MiniMath/Ast/Printer.hpp"
#include "MiniMath/Ast/Evaluator.hpp"

#include <fmt/format.h>

#include <iostream>
#include <string>

using namespace mm;
using namespace ast;
using namespace expr;

class StdinSource
{
public:
    [[nodiscard]]
    char getChar()
    {
        return buffer_[pos_++];
    }

    void restartBuffer()
    {
        pos_ = 0;
    }

    bool readStdin()
    {
        buffer_.clear();
        std::string line;

        while (buffer_.empty())
        {
            for (bool firstIt = true; !buffer_.ends_with(";;\n"); firstIt = false)
            {
                std::cout << (firstIt ? "> " : "- ");
                std::getline(std::cin, line);
                buffer_ += line + '\n';
            }

            buffer_.erase(buffer_.end() - 3, buffer_.end());
        }

        pos_ = 0;

        return buffer_ != "#quit";
    }

private:
    std::string buffer_;
    std::size_t pos_ = 0;
};

int main()
{
    Environment env;
    StdinSource charSource;
    Lexer lexer([&] { return charSource.getChar(); });
    MiniMathParser parser(lexer);

    std::cout << "MiniMath Repl\n\n";

    while (charSource.readStdin())
    {
        lexer.reset();

        try
        {
            auto stmt = parser.parseStatement();
            auto newSymbols = evaluate(stmt, env);
            parser.consume(TokenType::Eof);

            for (auto&& [name, val] : newSymbols)
                fmt::print("val {} = {}\n", name, val);

            env.merge(std::move(newSymbols));
        }
        catch (ParseError const& ex)
        {
            parser.resetState();
            lexer.reset();
            charSource.restartBuffer();
            try
            {
                auto root = parser.parseExpression();
                auto res = evaluate(root, env);
                fmt::print("= {}\n", res);

                env.add("ans", std::move(res));

                parser.consume(TokenType::Eof);
            }
            catch (std::exception const&)
            {
                std::cerr << ex.what() << "\n";
                parser.resetState();
            }
        }
        catch (std::exception const& ex)
        {
            std::cerr << ex.what() << "\n";
            parser.resetState();
            lexer.reset();
        }

        std::cout << '\n';
    }
}

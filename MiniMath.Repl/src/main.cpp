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

int main()
{
    std::string line;
    std::cout << "source: ";
    Environment env;

    while (std::getline(std::cin, line) && !line.empty())
    {
        try
        {
            Lexer lexer(line);
            MiniMathParser parser(lexer);

            auto root = parser.parseExpression();

            fmt::print("{}\n", root);

            auto res = evaluate(root, env);
            fmt::print("{}\n", res);

            env.add("ans", std::move(res));
        }
        catch (std::exception const& ex)
        {
            std::cerr << ex.what() << "\n";
        }

        std::cout << "\nsource: ";
    }
}
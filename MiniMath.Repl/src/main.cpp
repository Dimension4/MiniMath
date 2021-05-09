#include "MiniMath/MiniMathParser.hpp"
#include "MiniMath/Expressions.hpp"
#include "MiniMath/Ast/Printer.hpp"
#include "MiniMath/Ast/Evaluator.hpp"

#include <fmt/format.h>

#include <iostream>
#include <string>

using namespace mm;

int main()
{
    std::string line;
    std::cout << "source: ";

    while (std::getline(std::cin, line) && !line.empty())
    {
        try
        {
            Lexer lexer(line);
            MiniMathParser parser(lexer);

            auto root = parser.parseExpression();

            fmt::print("{}\n", root);

            auto res = ast::evaluate(root);
            fmt::print("{}\n", res);
        }
        catch (std::exception const& ex)
        {
            std::cerr << ex.what() << "\n";
        }

        std::cout << "\nsource: ";
    }
}
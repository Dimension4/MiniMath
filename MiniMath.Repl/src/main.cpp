#include "MiniMath/Lexer.hpp"
#include "MiniMath/MiniMathParser.hpp"
#include "MiniMath/PrattParser.hpp"

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

            std::cout << root->toString() << "\n";
        }
        catch (std::exception& ex)
        {
            std::cerr << ex.what() << "\n";
        }

        std::cout << "\nsource: ";
    }
}
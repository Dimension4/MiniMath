#include "MiniMath/Lexer.hpp"

#include <iostream>
#include <string>

using namespace mm;

int main()
{
    std::string line;
    std::cout << "source: ";

    while (std::getline(std::cin, line) && !line.empty())
    {
        Lexer lexer(line);

        while (true)
        {
            try
            {
                auto [type, lexeme] = lexer.nextToken();

                std::cout << lexeme << " (" << tokenName(type) << ") @ " << lexeme.data() - line.data() << '\n';

                if (type == TokenType::Eof)
                {
                    break;
                }
            }
            catch (std::exception& ex)
            {
                std::cerr << ex.what() << "\n";
            }
        }

        std::cout << "\nsource: ";
    }
}

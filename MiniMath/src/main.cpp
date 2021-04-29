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
                auto token = lexer.nextToken();

                if (token.type == TokenType::Eof)
                {
                    break;
                }

                std::cout << token.source << " @ " << token.source.data() - line.data() << '\n';
            }
            catch (std::exception& ex)
            {
                std::cerr << ex.what() << "\n";
            }
        }

        std::cout << "\nsource: ";
    }
}

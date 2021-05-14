#include "Repl/Repl.hpp"

#include <iostream>

int main()
{
    std::cout << "MiniMath REPL\nType '#help;;' to get some help.\n";
    mm::repl::Repl repl;
    repl.run();
}
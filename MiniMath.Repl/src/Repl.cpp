#include "Repl/Repl.hpp"

#include <fmt/format.h>

#include <iostream>
#include <algorithm>
#include <cctype>

void printHelp();
void printExample();

namespace mm::repl
{
    Repl::Repl() :
        lexer_([this] { return getNextChar(); }),
        parser_(lexer_)
    {
    }

    void Repl::run()
    {
        while (true)
        {
            std::cout << '\n';

            resetParser();
            readStdIn();

            if (inputBuffer_ == "#quit")
                break;

            if (inputBuffer_.starts_with('#'))
            {
                handleReplCommands();
                continue;
            }

            try
            {
                evalStatement();
            }
            catch (ParseError const& ex)
            {
                resetParser();

                try
                {
                    evalExpression();
                }
                catch (ParseError const&)
                {
                    std::cerr << ex.what() << "\n";
                }
                catch (std::exception const& ex2)
                {
                    std::cerr << ex2.what() << "\n";
                }
            }
            catch (std::exception const& ex)
            {
                std::cerr << ex.what() << "\n";
            }
        }
    }

    char Repl::getNextChar()
    {
        return inputBuffer_[bufferPos_++];
    }

    void Repl::readStdIn()
    {
        inputBuffer_.clear();
        std::string line;

        while (inputBuffer_.empty())
        {
            for (bool firstIt = true; !inputBuffer_.ends_with(";;\n"); firstIt = false)
            {
                std::cout << (firstIt ? "> " : "- ");
                std::getline(std::cin, line);
                inputBuffer_ += line + '\n';
            }

            inputBuffer_.erase(
                std::find_if_not(inputBuffer_.rbegin() + 3, inputBuffer_.rend(), &isspace).base(),
                inputBuffer_.end());
        }

        bufferPos_ = 0;
    }

    void Repl::handleReplCommands()
    {
        if (inputBuffer_ == "#reset")
        {
            env_ = {};
            fmt::print("Reset. All bindings have been cleared.\n");
        }
        else if (inputBuffer_ == "#env")
        {
            fmt::print("{}\n", env_);
        }
        else if (inputBuffer_ == "#example")
        {
            printExample();
        }
        else
        {
            printHelp();
        }
    }

    void Repl::evalStatement()
    {
        auto stmt = parser_.parseStatement();
        auto newSymbols = evaluate(stmt, env_);
        parser_.consume(TokenType::Eof);

        for (auto&& [name, val] : newSymbols)
            fmt::print("val {} = {}\n", name, val);

        env_.merge(std::move(newSymbols));
    }

    void Repl::evalExpression()
    {
        auto root = parser_.parseExpression();
        auto res = evaluate(root, env_);
        fmt::print("= {}\n", res);

        env_.add("ans", std::move(res));

        parser_.consume(TokenType::Eof);
    }

    void Repl::resetParser()
    {
        parser_.resetState();
        lexer_.reset();
        bufferPos_ = 0;
    }
}


void printHelp()
{
    fmt::print(R"(
Enter a statement or expression. End your input with ';;' and hit enter to submit it.
Special REPL commands are:

    #quit       exits the REPL.
    #env        shows the current environment
    #reset      resets the environment (clears all existing bindings).
    #help       shows this help.
    #example    shows some example code.

The language is very simple and only supports number types and everything is an expression.
The only exception are top level let-bindings (variable declarations).
)");
}

void printExample()
{
    fmt::print(R"(
Variable declaration:   let x = 10      
Function declaration:   let sum = fn a b -> a + b
Function calls:         foo(arg1 arg2 arg3 ...)

Complex variable declarations:

    let x = 
        let a = 10
        let b = foo(12) 
        a + b

Functions are values:

    let fancyAdd = fn x ->  // defines a function that returns a function
        fn y ->
            x + y

    fancyAdd(1)(2)      // returns 3

    let plus3 = f1(3)   // plus3: (num -> num)
    plus3(4)            // returns 7

Importing modules:

    import math.linalg  // imports ./math/linalg.mm

The 'import' directive imports all top level bindings from a module. 
A module is a code file with an *.mm extension.
Directories are separated by a '.' (dot) and are relative to the file that does the import.
If 'import' is used in the REPL, the current working directory is used as base path.
)");
}

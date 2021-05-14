#pragma once

#include "MiniMath/MiniMath.hpp"

#include <string>

namespace mm::repl
{
    class Repl
    {
    public:
        Repl();
        void run();
        
    private:
        ast::Environment env_;
        std::string inputBuffer_;
        std::size_t bufferPos_{};
        Lexer lexer_;
        MiniMathParser parser_;

        char getNextChar();
        void readStdIn();
        void handleReplCommands();
        void evalStatement();
        void evalExpression();
        void resetParser();        
    };
}

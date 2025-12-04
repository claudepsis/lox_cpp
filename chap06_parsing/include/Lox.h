#ifndef LOX_H
#define Lox_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "Token.h"
class Lox{
    inline static bool hadError=false;
    public:
        static void runFile(const std::string& path);
        static void runPrompt();
        static void run(const std::string& source);
        static void error(int line, const std::string& message);
        static void error(Token token,const std::string& mesage);
        static void report(int line,const std::string& where,const std::string& message);
};

#endif

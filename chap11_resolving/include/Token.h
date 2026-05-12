#ifndef TOKEN_H
#define TOKEN_H
#include "TokenType.h"
#include <string>
#include <any>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
class Token
{
public:
    const TokenType type;
    const std::string lexeme;
    const std::any literal;
    const int line;
    Token(TokenType type, std::string lexeme, std::any literal, int line) : type(type), lexeme(lexeme), literal(literal), line(line) {};
    std::string toString();
};

std::string formatDouble(const double &number);
#endif

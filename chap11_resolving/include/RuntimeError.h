#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H
#include <stdexcept>
#include "Token.h"
#include <string>
class RuntimeError:std::runtime_error{
public:
    const Token token;
    const std::string message;
    RuntimeError(const Token &token,const std::string &message):std::runtime_error(message),token(std::move(token)),message(message){};
};


#endif
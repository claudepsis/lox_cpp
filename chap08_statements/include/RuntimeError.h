#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H
#include <stdexcept>
#include "Token.h"
#include <string>
class RuntimeError:std::runtime_error{
public:
    const Token token;
    const std::string message;
    RuntimeError(const Token &token,const std::string &message):token(std::move(token)),std::runtime_error(message),message(message){};
};


#endif
#ifndef ENVIROMENT_H
#define ENVIROMENT_H
#include <unordered_map>
#include <any>
#include <string>
#include <map>
#include "Token.h"
#include "RuntimeError.h"
class Environment{
    std::unordered_map<std::string,std::any> values;
public:
    void define(const std::string &name,std::any &value);
    std::any get(const Token& name);
    void assign(Token &name,std::any &value);
};


#endif
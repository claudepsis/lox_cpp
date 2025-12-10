#include "Environment.h"

void Environment::define(const std::string &name,std::any &value){
    values.insert(std::make_pair(name,value));
}

std::any Environment::get(const Token&name){
    auto it=values.find(name.lexeme);
    if(it==values.end()) throw RuntimeError(name,"Undefined variable '" + name.lexeme + "'.");
    return it->second;
}

void Environment::assign(Token &name,std::any &value){
    auto it=values.find(name.lexeme);
    if(it==values.end()) throw RuntimeError(name,"Undefined variable '" + name.lexeme + "'.");
    it->second=value;
    return ;
}
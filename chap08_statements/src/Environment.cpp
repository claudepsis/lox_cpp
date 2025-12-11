#include "Environment.h"

void Environment::define(const std::string &name,std::any &value){
    //values.insert_or_assign(name,value);
    values[name]=value;
}

std::any Environment::get(const Token&name){
    auto it=values.find(name.lexeme);
    if(it!=values.end()) return it->second;
    if(enclosing!=nullptr){
        return enclosing->get(name);
    }
    throw RuntimeError(name,"Undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(Token &name,std::any &value){
    auto it=values.find(name.lexeme);
    if(it!=values.end()){
        it->second=value;
        return ;
    }
    if(enclosing!=nullptr){
        enclosing->assign(name,value);
        return ;
    } 
    throw RuntimeError(name,"Undefined variable '" + name.lexeme + "'.");
}

Environment::Environment(){
    enclosing=nullptr;
}

Environment::Environment(std::shared_ptr<Environment> enclosing){
    this->enclosing=enclosing;
}
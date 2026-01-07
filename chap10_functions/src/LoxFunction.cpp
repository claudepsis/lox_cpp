#include "LoxFunction.h"



std::any LoxFunction::call(Interpreter &interpreter,std::vector<std::any>&arguments){
    Environment enviornment=Environment(interpreter.globals);
    for(int i=0;i<arguments.size();i++){
        enviornment.define(declaration.params[i].lexeme,arguments[i]);
    }
    interpreter.executeBlock(declaration.body,std::make_shared<Environment>(enviornment));
    return std::any();
}

int LoxFunction::arity() const{
    return declaration.params.size();
}

std::string LoxFunction::toString() const{
    return "<fn " + declaration.name.lexeme + ">";
}
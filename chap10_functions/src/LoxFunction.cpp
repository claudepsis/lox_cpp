#include "LoxFunction.h"
#include "ReturnVal.h"


std::any LoxFunction::call(Interpreter &interpreter,std::vector<std::any>&arguments){
    //must bn
    std::shared_ptr<Environment> enviornment=std::make_shared<Environment>(closure); 
    for(int i=0;i<arguments.size();i++){
        enviornment->define(declaration.params[i].lexeme,arguments[i]);
    }
    try{
        interpreter.executeBlock(declaration.body,enviornment);
    }catch(ReturnVal returnval){
        return returnval.value;
    }
    
    return std::any();
}

int LoxFunction::arity() const{
    return declaration.params.size();
}

std::string LoxFunction::toString() const{
    return "<fn " + declaration.name.lexeme + ">";
}
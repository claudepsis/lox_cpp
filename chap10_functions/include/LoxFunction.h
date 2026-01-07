#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H
#include "LoxCallable.h"
#include <any>
#include "Interpreter.h"
#include <memory>
class LoxFunction:LoxCallable{
    public:
    Function declaration;
    LoxFunction(Function &declaration):declaration(declaration){};

    std::any call(Interpreter &interpreter,std::vector<std::any>&arguments) override;
    int arity() const override;
    std::string toString() const override;
};


#endif


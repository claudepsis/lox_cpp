#ifndef LOXFUNCTION_H
#define LOXFUNCTION_H
#include "LoxCallable.h"
#include <any>
#include "Interpreter.h"
#include <memory>
class LoxFunction:public LoxCallable{
    public:
    Function declaration;
    std::shared_ptr<Environment> closure;
    LoxFunction(Function &declaration,std::shared_ptr<Environment> closure):declaration(declaration),closure(closure){};

    std::any call(Interpreter &interpreter,std::vector<std::any>&arguments) override;
    int arity() const override;
    std::string toString() const override;
};


#endif


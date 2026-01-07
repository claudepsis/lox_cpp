#ifndef LOXCALLABLE_H
#define LOXCALLABLE_H
#include <vector>
#include "Interpreter.h"
#include <any>

class LoxCallable
{
private:
    /* data */
public:
    virtual ~LoxCallable()=default;
    virtual int arity() const=0;
    virtual std::any call(Interpreter& Interpreter,std::vector<std::any> &arguments)=0;
    virtual std::string toString() const =0;
};


std::shared_ptr<LoxCallable>  createClockFunction();






#endif
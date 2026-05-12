#ifndef RETURNVAL_H 
#define RETURNVAL_H
#include <any>
#include <exception>


class ReturnVal :public std::exception{
public:
    std::any value;
    ReturnVal(std::any value):value(value){};
};


#endif

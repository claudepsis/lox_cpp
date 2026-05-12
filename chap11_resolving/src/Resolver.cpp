#include "Reslover.h"
#include <Lox.h>


void Resolver::resolve(Stmt& stmt){
    stmt.accept(*this);
}

void Resolver::resolve(Expr& expr){
    expr.accept(*this);
}

void Resolver::resolve(std::vector<std::shared_ptr<Stmt>>&statments){
    for(auto stmt:statments){
        resolve(*stmt);
    }
}

void Resolver::beginScope(){
    scopes.push_back(std::map<std::string,bool>());
}

void Resolver::endScope(){
    scopes.pop_back();
}

std::any Resolver::visitBlockStmt(Block& stmt){
    beginScope();
    resolve(stmt.statements);
    endScope();
    return ;   
}

void Resolver::declare(Token &name){
    if(scopes.empty()) return;
    std::map<std::string,bool> &scope=*scopes.begin();
    scope.insert(make_pair(name.lexeme,false));
}

void Resolver::define(Token& name){
    if(scopes.empty()) return;
    scopes.begin()->insert(std::make_pair(name.lexeme,true));
}

std::any Resolver::visitVarStmt(Var& stmt){
    declare(stmt.name);
    if(stmt.initializer!=nullptr){
        resolve(*stmt.initializer);
    }
    define(stmt.name);
    return std::any();
}

void Resolver::resolveLocal(Expr &expr,Token&name){
    for(int i=scopes.size()-1;i>=0;i--){
        if(scopes[i].count(name.lexeme)){
            _interpreter.resolve(expr,scopes.size()-1-i);
            return ;
        }
    }
}


void Resolver::resolveFunction(Function &function){
    beginScope();
    for(Token &param:function.params){
        declare(param);
        define(param);
    }
    resolve(function.body);
    endScope();
}

std::any Resolver::visitFunctionStmt(Function& stmt){
    declare(stmt.name);
    define(stmt.name);
    resolveFunction(stmt);
    return std::any();
}


std::any Resolver::visitAssignExpr(Assign &expr){
    resolve(*expr.value);
    resolveLocal(expr,expr.name);
    return std::any();
}

std::any Resolver::visitVariableExpr(Variable& expr){
    if(!scopes.empty()&&scopes.front()[expr.name.lexeme]==false){
        Lox::error(expr.name,"Can't read local variable in its own initializer.");
    }
    resolveLocal(expr,expr.name);
    return std::any();
}
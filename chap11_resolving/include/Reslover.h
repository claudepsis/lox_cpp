#ifndef RESLOVER_H
#define RESLOVER_H
#include "Expr.h"
#include "Interpreter.h"
#include "Stmt.h"
#include <memory>
#include <vector>
#include <queue>
#include <map>
#include <string>
class Resolver:public Expr::Visitor,public Stmt::Visitor{
    Interpreter& _interpreter;
    std::deque<std::map<std::string,bool>> scopes;
    

    void resolve(Stmt &stmt);
    void resolve(Expr &expr);
    void beginScope();
    void endScope();
    void declare(Token &name);
    void define(Token &name);
    std::any visitVarStmt(Var &stmt) override;
    std::any visitVariableExpr(Variable &expr) override;
    void resolveLocal(Expr &expr,Token &name);
    void resolveFunction(Function &function);
public:
    std::any visitBlockStmt(Block& stmt);
    std::any visitAssignExpr(Assign& expr);
    std::any  visitFunctionStmt(Function &stmt);
    void resolve(std::vector<std::shared_ptr<Stmt>> &statments);
    Resolver(Interpreter& interpreter):_interpreter(interpreter){}

};


#endif

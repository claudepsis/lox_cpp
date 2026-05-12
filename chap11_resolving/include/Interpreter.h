#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Expr.h"
#include "Stmt.h"
#include "RuntimeError.h"
#include <any>
#include <memory>
#include <string>
#include <vector>
#include "Environment.h"
class Interpreter : public Expr::Visitor,public Stmt::Visitor
{
public:
    // 访问者模式方法
    std::any visitLiteralExpr(Literal &expr) override;
    std::any visitGroupingExpr(Grouping &expr) override;
    std::any visitUnaryExpr(Unary &expr) override;
    std::any visitBinaryExpr(Binary &expr) override;
    std::any visitTenaryExpr(Tenary &expr) override;
    std::any visitVariableExpr(Variable&expr) override;
    std::any visitAssignExpr(Assign &expr) override;
    std::any visitLogicalExpr(Logical &expr) override;
    std::any visitCallExpr(Call& expr) override;
    std::any visitExpressionStmt(Expression &stmt) override;
    std::any visitPrintStmt(Print &stmt) override;
    std::any visitIfStmt(If &stmt);
    std::any visitVarStmt(Var &stmt) override;
    std::any visitBlockStmt(Block& stmt) override;
    std::any visitWhileStmt(While& stmt) override;
    std::any visitFunctionStmt(Function& stmt)override;
    std::any visitReturnStmt(Return &stmt) override;
    // 解释执行
    void interpret(std::vector<std::unique_ptr<Stmt>>& statements);
    Interpreter();
    std::shared_ptr<Environment> globals=std::make_shared<Environment>(Environment());
    std::shared_ptr<Environment> environment=globals; 
    void executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements,
                     std::shared_ptr<Environment> blockEnvironment);
private:

    void execute(Stmt& stmt);
    // 辅助方法
    std::any evaluate(std::unique_ptr<Expr> &expr);
    std::any evaluate(std::shared_ptr<Expr> &expr);
    bool isEqual(Object& a, Object &b);
    std::string stringify(std::any object);
    bool isTruthy(const Object &object);
    void checkNumberOperand(Token &op, Object &operand);
    void checkNumberOperands(Token &op, Object &left, Object &right);
    std::string formatDouble(const double&num);


};

#endif // INTERPRETER_H

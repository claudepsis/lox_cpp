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
    std::any visitExpressionStmt(Expression &stmt) override;
    std::any visitPrintStmt(Print &stmt) override;
    std::any visitVarStmt(Var &stmt) override;
    // 解释执行
    void interpret(std::vector<std::unique_ptr<Stmt>>& statements);
    
private:
    Environment environment=Environment();
    void execute(Stmt& stmt);
    // 辅助方法
    std::any evaluate(std::unique_ptr<Expr> &expr);
    bool isEqual(Object& a, Object &b);
    std::string stringify(std::any object);
    bool isTruthy(Object &object);
    void checkNumberOperand(Token &op, Object &operand);
    void checkNumberOperands(Token &op, Object &left, Object &right);
    std::string formatDouble(const double&num);
};

#endif // INTERPRETER_H

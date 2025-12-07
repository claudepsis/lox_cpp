#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Expr.h"
#include "RuntimeError.h"
#include <any>
#include <memory>
#include <string>

class Interpreter : public Visitor
{
public:
    // 访问者模式方法
    std::any visitLiteralExpr(Literal &expr) override;
    std::any visitGroupingExpr(Grouping &expr) override;
    std::any visitUnaryExpr(Unary &expr) override;
    std::any visitBinaryExpr(Binary &expr) override;
    std::any visitTenaryExpr(Tenary &expr) override;
    // 解释执行
    void interpret(std::unique_ptr<Expr> expression);
    
private:
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

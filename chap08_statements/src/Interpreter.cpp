#include "Interpreter.h"
#include "Lox.h"
#include <iostream>
#include <sstream>
#include <iomanip>

std::any Interpreter::visitLiteralExpr(Literal &expr)
{
    return expr.value;
}

std::any Interpreter::visitExpressionStmt(Expression &stmt)
{
    evaluate(stmt.expression);
    return nullptr;
}

//var stmt has no value
std::any Interpreter::visitVarStmt(Var &stmt){
    std::any value=nullptr;
    if(stmt.initializer!=nullptr){
        value=evaluate(stmt.initializer);
    }
    environment.define(stmt.name.lexeme,value);
    return nullptr;
}

std::any Interpreter::visitAssignExpr(Assign& expr){
    std::any value=evaluate(expr.value);
    environment.assign(expr.name,value);
    return value;
}

std::any Interpreter::visitVariableExpr(Variable &expr){
    return environment.get(expr.name);
}



std::any Interpreter::visitPrintStmt(Print &stmt)
{
    std::any value = evaluate(stmt.expression);
    std::cout << stringify(value) << std::endl;
    return nullptr;
}

std::any Interpreter::visitGroupingExpr(Grouping &expr)
{
    return evaluate(expr.expression);
}

std::any Interpreter::visitUnaryExpr(Unary &expr)
{
    std::any right = evaluate(expr.right);

    switch (expr.op.type)
    {
    case MINUS:
        checkNumberOperand(expr.op, right);
        return -(std::any_cast<double>(right));
    case BANG:
        return !isTruthy(right);
    default:
        break;
    }

    return nullptr;
}

//this can confirm 
std::any Interpreter::visitTenaryExpr(Tenary &expr)
{
    std::any condition = evaluate(expr.condtion);
    if (isTruthy(condition))
        return evaluate(expr.thenBranch);
    return evaluate(expr.elseBranch);
}

std::any Interpreter::visitBinaryExpr(Binary &expr)
{
    std::any left = evaluate(expr.left);
    std::any right = evaluate(expr.right);

    switch (expr.op.type)
    {
    case COMMA:
        return right;

    case MINUS:
        checkNumberOperands(expr.op, left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);

    case SLASH:
        checkNumberOperands(expr.op, left, right);
        if (std::any_cast<double>(right) == 0.0)
            throw RuntimeError(expr.op, "Division by zero.");
        return std::any_cast<double>(left) / std::any_cast<double>(right);

    case STAR:
        checkNumberOperands(expr.op, left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);

    case PLUS:
    {
        if (left.type() == typeid(double) && right.type() == typeid(double))
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        if (left.type() == typeid(std::string) || right.type() == typeid(std::string))
            return stringify(left) + stringify(right);
        throw RuntimeError(expr.op, "Operands must be two numbers or two strings.");
    }

    case GREATER:
        checkNumberOperands(expr.op, left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);

    case GREATER_EQUAL:
        checkNumberOperands(expr.op, left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);

    case LESS:
        checkNumberOperands(expr.op, left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);

    case LESS_EQUAL:
        checkNumberOperands(expr.op, left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);

    case BANG_EQUAL:
        return !isEqual(left, right);

    case EQUAL_EQUAL:
        return isEqual(left, right);

    default:
        break;
    }

    return nullptr;
}

void Interpreter::interpret(std::vector<std::unique_ptr<Stmt>> &statements)
{
    try
    {
        for (std::unique_ptr<Stmt> &statement : statements)
        {
            execute(*statement);
        }
    }
    catch (RuntimeError error)
    {
        Lox::runtimeError(error);
    }
}

void Interpreter::execute(Stmt &stmt)
{
    stmt.accept(*this);
}

std::any Interpreter::evaluate(std::unique_ptr<Expr> &expr)
{
    return expr->accept(*this);
}

bool Interpreter::isEqual(Object &a, Object &b)
{
    if (!a.has_value() && !b.has_value())
        return true;
    if (!a.has_value() || !b.has_value())
        return false;
    if (a.type() != b.type())
        return false;

    if (a.type() == typeid(double))
    {
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    }
    else if (a.type() == typeid(std::string))
    {
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }
    else if (a.type() == typeid(bool))
    {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }

    return false;
}

std::string Interpreter::formatDouble(const double &number)
{
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

std::string Interpreter::stringify(std::any object)
{
    if (!object.has_value())
        return "nil";

    if (object.type() == typeid(double))
        return formatDouble(std::any_cast<double>(object));

    if (object.type() == typeid(std::string))
        return std::any_cast<std::string>(object);

    if (object.type() == typeid(bool))
        return std::any_cast<bool>(object) ? "true" : "false";

    return "unknown";
}

bool Interpreter::isTruthy(Object &object)
{
    if (!object.has_value())
        return false;
    if (object.type() == typeid(bool))
        return std::any_cast<bool>(object);
    return true;
}

void Interpreter::checkNumberOperand(Token &op, Object &operand)
{
    if (operand.type() == typeid(double))
        return;
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::checkNumberOperands(Token &op, Object &left, Object &right)
{
    if (left.type() == typeid(double) && right.type() == typeid(double))
        return;
    throw RuntimeError(op, "Operands must be numbers.");
}

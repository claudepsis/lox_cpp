#ifndef Expr_H
#define Expr_H
#include<memory>
#include<any>
#include "Token.h"
typedef std::any Object;
class Binary;
class Grouping;
class Literal;
class Unary;
class Visitor{
public:
	virtual ~Visitor()=default;
	virtual std::any visitBinaryExpr(Binary& expr)=0;
	virtual std::any visitGroupingExpr(Grouping& expr)=0;
	virtual std::any visitLiteralExpr(Literal& expr)=0;
	virtual std::any visitUnaryExpr(Unary& expr)=0;
};

class Expr{
public:
	virtual ~Expr()=default;
	virtual std::any accept(Visitor& visitor);
};

class Binary : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
};

class Grouping : public Expr {
public:
    std::unique_ptr<Expr> expression;

    Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
};

class Literal : public Expr {
public:
    Object value;

    Literal(Object value)
        : value(std::move(value)) {}
};

class Unary : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}
};

#endif

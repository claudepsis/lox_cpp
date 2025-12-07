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
class Tenary;
class Visitor{
public:
	virtual ~Visitor()=default;
	virtual std::any visitBinaryExpr(Binary& expr)=0;
	virtual std::any visitGroupingExpr(Grouping& expr)=0;
	virtual std::any visitLiteralExpr(Literal& expr)=0;
	virtual std::any visitUnaryExpr(Unary& expr)=0;
	virtual std::any visitTenaryExpr(Tenary& expr)=0;
};

class Expr{
public:
	virtual ~Expr()=default;
	virtual std::any accept(Visitor& visitor)=0;
};

class Binary : public Expr {
public:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitBinaryExpr(*this);
	}
};

class Grouping : public Expr {
public:
    std::unique_ptr<Expr> expression;

    Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitGroupingExpr(*this);
	}
};

class Literal : public Expr {
public:
    Object value;

    Literal(Object value)
        : value(std::move(value)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitLiteralExpr(*this);
	}
};

class Unary : public Expr {
public:
    Token op;
    std::unique_ptr<Expr> right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitUnaryExpr(*this);
	}
};

class Tenary : public Expr {
public:
    std::unique_ptr<Expr> condtion;
    std::unique_ptr<Expr> thenBranch;
    std::unique_ptr<Expr> elseBranch;

    Tenary(std::unique_ptr<Expr> condtion, std::unique_ptr<Expr> thenBranch, std::unique_ptr<Expr> elseBranch)
        : condtion(std::move(condtion)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitTenaryExpr(*this);
	}
};

#endif

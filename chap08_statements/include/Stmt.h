#ifndef Stmt_H
#define Stmt_H
#include<memory>
#include<any>
#include "Token.h"
typedef std::any Object;
class Expression;
class Print;
class Var;
class Stmt{
public:
class Visitor{
public:
	virtual ~Visitor()=default;
	virtual std::any visitExpressionStmt(Expression& stmt)=0;
	virtual std::any visitPrintStmt(Print& stmt)=0;
	virtual std::any visitVarStmt(Var& stmt)=0;
};

	virtual ~Stmt()=default;
	virtual std::any accept(Visitor& visitor)=0;
};

class Expression : public Stmt {
public:
    std::unique_ptr<Expr> expression;

    Expression(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitExpressionStmt(*this);
	}
};

class Print : public Stmt {
public:
    std::unique_ptr<Expr> expression;

    Print(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitPrintStmt(*this);
	}
};

class Var : public Stmt {
public:
    Token name;
    std::unique_ptr<Expr> initializer;

    Var(Token name, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitVarStmt(*this);
	}
};

#endif

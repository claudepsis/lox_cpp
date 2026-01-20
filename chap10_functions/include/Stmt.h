#ifndef Stmt_H
#define Stmt_H
#include<memory>
#include<any>
#include "Token.h"
typedef std::any Object;
class Expression;
class Function;
class If;
class Print;
class Return;
class Var;
class Block;
class While;
class Stmt{
public:
class Visitor{
public:
	virtual ~Visitor()=default;
	virtual std::any visitExpressionStmt(Expression& stmt)=0;
	virtual std::any visitFunctionStmt(Function& stmt)=0;
	virtual std::any visitIfStmt(If& stmt)=0;
	virtual std::any visitPrintStmt(Print& stmt)=0;
	virtual std::any visitReturnStmt(Return& stmt)=0;
	virtual std::any visitVarStmt(Var& stmt)=0;
	virtual std::any visitBlockStmt(Block& stmt)=0;
	virtual std::any visitWhileStmt(While& stmt)=0;
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

class Function : public Stmt {
public:
    Token name;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;

    Function(Token name,std::vector<Token> params,std::vector<std::shared_ptr<Stmt>> body)
        : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitFunctionStmt(*this);
	}
};

class If : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;

    If(std::unique_ptr<Expr> condition,std::unique_ptr<Stmt> thenBranch,std::unique_ptr<Stmt> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitIfStmt(*this);
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

class Return : public Stmt {
public:
    Token keyword;
    std::unique_ptr<Expr> value;

    Return(Token keyword,std::unique_ptr<Expr> value)
        : keyword(std::move(keyword)), value(std::move(value)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitReturnStmt(*this);
	}
};

class Var : public Stmt {
public:
    Token name;
    std::unique_ptr<Expr> initializer;

    Var(Token name,std::unique_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitVarStmt(*this);
	}
};

class Block : public Stmt {
public:
    std::vector<std::shared_ptr<Stmt>> statements;

    Block(std::vector<std::shared_ptr<Stmt>> statements)
        : statements(std::move(statements)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitBlockStmt(*this);
	}
};

class While : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;

    While(std::unique_ptr<Expr> condition,std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)) {}
	std::any accept(Visitor& visitor){
		return visitor.visitWhileStmt(*this);
	}
};

#endif

#ifndef PARSER_H
#define PARSER_H
#include <vector>
#include <list>
#include "Token.h"
#include "Expr.h"
#include <memory>
#include "Stmt.h"
class Parser{
    int current=0;
    std::vector<Token> tokens;
    
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> comma();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> tenary();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    std::unique_ptr<Expr> logicOr();
    std::unique_ptr<Expr> logicAnd();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> printStmt();
    std::unique_ptr<Stmt> ifStmt();
    std::unique_ptr<Stmt> exprStmt();
    std::unique_ptr<Stmt> delcaration();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> whileStmt();
    std::unique_ptr<Stmt> forStmt();
    std::vector<std::shared_ptr<Stmt>> block();
    Token consume(TokenType type,const std::string& message);
    template<typename ...TokenTypes>
    bool match(TokenTypes ... types){
        for(TokenType type:{types...}){
            if(check(type)){
                advance();
                return true;
            }
        }
        return false;
    }
    bool check(TokenType type);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous(); 

    void synchronize();
    class ParseError:public std::runtime_error{
        public:
            explicit ParseError(const std::string&message):std::runtime_error(message){}
    };
    public:
    std::vector<std::unique_ptr<Stmt>> parse();
    Parser(std::vector<Token> tokens):tokens(tokens){}
    ParseError error(Token token,const std::string message);
};

#endif
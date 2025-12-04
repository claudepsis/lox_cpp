#include "Parser.h"
#include "Lox.h"

using namespace std;


bool Parser::check(TokenType type){
    if(isAtEnd()) return false;
    return  peek().type==type;
}


Token Parser::peek(){
    return tokens.at(current);
}

bool Parser::isAtEnd(){
    return peek().type==EOF_TOKEN;
}

Token Parser::advance(){
    if(!isAtEnd()) ++current;
    return previous();
}

Token Parser::previous(){
    return tokens.at(current-1);
}

std::unique_ptr<Expr> Parser::expression(){
    return equality();
}

std::unique_ptr<Expr> Parser::equality(){
    unique_ptr<Expr> expr=comparison();
    while(match(BANG_EQUAL,EQUAL_EQUAL)){
        Token op=previous();
        unique_ptr<Expr> right=comparison();
        expr=make_unique<Binary>(std::move(expr),op,move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::comparison(){
    unique_ptr<Expr>  expr=term();
    while(match(GREATER,GREATER_EQUAL,LESS,LESS_EQUAL)){
        Token op=previous();
        unique_ptr<Expr> right=term();
        expr=make_unique<Binary>(move(expr),op,move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term(){
    unique_ptr<Expr> expr=factor();
    while(match(MINUS,PLUS)){
        Token op=previous();
        unique_ptr<Expr> right=factor();
        expr=make_unique<Binary>(move(expr),op,move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor(){
    unique_ptr<Expr> expr=unary();
    while(match(SLASH,STAR)){
        Token op=previous();
        unique_ptr<Expr> right=unary();
        expr=make_unique<Binary>(move(expr),op,move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::unary(){
    if(match(BANG,MINUS)){
        Token op=previous();
        unique_ptr<Expr> right=unary();
        return make_unique<Unary>(op,move(right));
    }
    return primary();
}



void Parser::synchronize(){
    advance();
    while (!isAtEnd()) {
      if (previous().type == SEMICOLON) return;
      switch (peek().type) {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
          return;
      }
      advance();
    }
}

Token Parser::consume(TokenType type,const std::string&message){
    if(check(type)) return advance();
    throw ParseError(message);
}

Parser::ParseError Parser::error(Token token,const std::string message){
    Lox::error(token,message);
    return ParseError(message);
}

std::unique_ptr<Expr> Parser::parse(){
    try{
        return expression(); 
    }catch(ParseError error){
        return nullptr;
    }
}

std::unique_ptr<Expr> Parser::primary(){
    if(match(FALSE)) return make_unique<Literal>(false);
    if(match(TRUE))  return make_unique<Literal>(true);
    if(match(NIL))  return make_unique<Literal>(nullptr);
    if(match(NUMBER,STRING)) return make_unique<Literal>(previous().literal);
    if(match(LEFT_PAREN)){
        std::unique_ptr<Expr> expr=expression();
        consume(RIGHT_PAREN,"Expect ')' after expression");
        return make_unique<Grouping>(std::move(expr));
    }
    throw error(peek(),"Expect expression");
}
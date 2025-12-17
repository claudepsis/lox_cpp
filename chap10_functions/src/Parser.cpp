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
    return comma();
}



std::unique_ptr<Expr> Parser::comma(){
    unique_ptr<Expr> expr=tenary();
    while(match(COMMA)){
        Token op=previous();
        unique_ptr<Expr> right=tenary();
        expr=make_unique<Binary>(std::move(expr),op,move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::call(){
    std::unique_ptr<Expr> expr=primary();
    while(true){
        if(match(LEFT_PAREN)){
            expr=finishCall(expr);
        }
        else break;
    }
    return std::move(expr);
}

std::unique_ptr<Expr> Parser::finishCall(const Expr&callee){
    std::vector<std::shared_ptr<Expr>> arguments;
    while(!match(RIGHT_PAREN)){
        std::shared_ptr<Expr> expr=make_shared<Expr>(expression());
        arguments.push_back(expr);
    }
}

std::unique_ptr<Expr> Parser::tenary(){
    unique_ptr<Expr> expr=logicOr();
    if(match(QUESTION)){
        unique_ptr<Expr> thenBranch=expression();
        consume(COLON,"expect : in tenary expression.");
        unique_ptr<Expr> elseBranch=tenary();
        expr=make_unique<Tenary>(std::move(expr),std::move(thenBranch),std::move(elseBranch)); 
    }
    return expr;
}

std::unique_ptr<Expr> Parser::assignment(){
    unique_ptr<Expr> expr=equality();
    if(match(EQUAL)){
        Token equals=previous();
        unique_ptr<Expr> value=assignment();
        if(auto var=dynamic_cast<Variable*>(expr.get())){
            return make_unique<Assign>(var->name,std::move(value));
        }
        Lox::error(equals,"Invalid assignment target.");
    }
    return expr;
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
    return call();
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
    throw error(peek(),message);  
}

Parser::ParseError Parser::error(Token token,const std::string message){
    Lox::error(token,message);
    return ParseError(message);
}

std::unique_ptr<Stmt> Parser::statement(){
    if(match(PRINT)) return printStmt();
    if(match(IF)) return ifStmt();
    if(match(FOR)) return forStmt();
    if(match(WHILE)) return whileStmt();
    if(match(LEFT_BRACE)) return make_unique<Block>(block());
    else return exprStmt();
}

std::unique_ptr<Stmt> Parser::forStmt(){
     consume(LEFT_PAREN, "Expect '(' after 'for'.");
     std::unique_ptr<Stmt> initializer=nullptr;
     if(match(SEMICOLON)){
        initializer=nullptr;
     }else if(match(VAR)){
        initializer=varDeclaration();
     }
     else{
        initializer=exprStmt();
     }
     std::unique_ptr<Expr> condition=nullptr;
     if(!check(SEMICOLON)){
        condition=expression();
     }
     consume(SEMICOLON, "Expect ';' after loop condition.");
     std::unique_ptr<Expr> increment=nullptr;
     if(!check(RIGHT_PAREN)){
        increment=expression();
     }
     consume(RIGHT_PAREN,"Expect ')' after for clauses.");
     std::unique_ptr<Stmt> body=statement();
     if(increment != nullptr) {
        std::vector<std::shared_ptr<Stmt>> stmts;
        stmts.push_back(std::shared_ptr<Stmt>(std::move(body)));
        stmts.push_back(std::make_shared<Expression>(std::move(increment)));
        body = std::make_unique<Block>(std::move(stmts));
    }
    if(condition==nullptr) condition=make_unique<Literal>(true);
    body=std::make_unique<While>(std::move(condition),std::move(body));
    if(initializer!=nullptr){
        std::vector<std::shared_ptr<Stmt>> stmts;
        stmts.push_back(std::shared_ptr<Stmt>(std::move(initializer)));
        stmts.push_back(std::shared_ptr<Stmt>(std::move(body)));
        body=std::make_unique<Block>(std::move(stmts));
    }
    return body;
}

std::unique_ptr<Stmt> Parser::whileStmt(){
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expr> condition=expression();
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    std::unique_ptr<Stmt> body=statement();
    return make_unique<While>(std::move(condition),std::move(body));
}

std::vector<std::shared_ptr<Stmt>> Parser::block(){
    vector<shared_ptr<Stmt>> statements;
    while(!check(RIGHT_BRACE)&&!isAtEnd()){
        statements.push_back(delcaration());
    }
    consume(RIGHT_BRACE,"Expect ')' after block.");
    return statements;
}

std::unique_ptr<Stmt> Parser::printStmt(){
    std::unique_ptr<Expr> value=expression();
    consume(SEMICOLON,"Expect ';' after expression.");
    return make_unique<Print>(std::move(value));
}

std::unique_ptr<Stmt> Parser::ifStmt(){
    consume(LEFT_PAREN,"Expect '(' after 'if'.");
    std::unique_ptr<Expr> condition=expression();
    consume(RIGHT_PAREN,"Expect ')' after if condition.");
    std::unique_ptr<Stmt> thenBranch=statement();
    std::unique_ptr<Stmt> elseBranch=nullptr;
    if(match(ELSE)){
        elseBranch=statement();
    }
    return make_unique<If>(std::move(condition),std::move(thenBranch),std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::varDeclaration(){
    Token name=consume(IDENTIFIER,"Expect variable name.");
    std::unique_ptr<Expr> initializer;
    if(match(EQUAL)){
        initializer=expression();
    }
    consume(SEMICOLON,"Expect ';' afeter variable declaration.");
    return make_unique<Var>(name,std::move(initializer));
}

std::unique_ptr<Expr> Parser::logicOr(){
    std::unique_ptr<Expr> expr=logicAnd();
    while(match(OR)){
        Token op=previous();
        std::unique_ptr<Expr> right=logicAnd();
        expr=make_unique<Logical>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<Expr> Parser::logicAnd(){
    std::unique_ptr<Expr> expr=assignment();
    while(match(AND)){
        Token op=previous();
        std::unique_ptr<Expr> right=assignment();
        expr=make_unique<Logical>(std::move(expr),op,std::move(right));
    }
    return expr;
}

std::unique_ptr<Stmt> Parser::delcaration(){
    try{
        if(match(VAR)) return varDeclaration();
        return  statement();
    }catch(ParseError error){
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::exprStmt(){
    std::unique_ptr<Expr> value=expression();
    consume(SEMICOLON,"Expect ';' after expression.");
    return make_unique<Expression>(std::move(value));
}

std::vector<std::unique_ptr<Stmt>> Parser::parse(){
    std::vector<std::unique_ptr<Stmt>> statements;
    while(!isAtEnd()){
        // auto stmt=delcaration();
        // if(stmt!=nullptr)statements.push_back(std::move(stmt));
        statements.push_back(delcaration());
    }
    return statements;
}

std::unique_ptr<Expr> Parser::primary(){
    if(match(FALSE)) return make_unique<Literal>(false);
    if(match(TRUE))  return make_unique<Literal>(true);
    if(match(NIL))  return make_unique<Literal>(std::any());
    if(match(NUMBER,STRING)) return make_unique<Literal>(previous().literal);
    if(match(LEFT_PAREN)){
        std::unique_ptr<Expr> expr=expression();
        consume(RIGHT_PAREN,"Expect ')' after expression");
        return make_unique<Grouping>(std::move(expr));
    }
    if(match(IDENTIFIER)){
        return make_unique<Variable>(previous());
    }
    throw error(peek(),"Expect expression.");
}
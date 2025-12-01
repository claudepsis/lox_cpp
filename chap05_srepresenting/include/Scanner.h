#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include "Token.h"

class Scanner {
public:
    // 构造函数
    explicit Scanner(const std::string& source);
    
    // 公有方法
    std::vector<Token> scanTokens();
    
private:
    // 成员变量
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
    
    // 静态关键字映射
    static const std::unordered_map<std::string, TokenType> keywords;
    
    // 私有方法 - 扫描相关
    bool isAtEnd() const;
    void scanToken();
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    
    // 私有方法 - 字面量处理
    void loxString();
    void number();
    void identifier();
    
    // 私有方法 - 字符判断
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    
    // 私有方法 - Token 添加
    void addToken(TokenType type);
    void addToken(TokenType type, std::any literal);
};

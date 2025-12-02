#include "Scanner.h"
#include "Lox.h"

// 初始化静态成员
const std::unordered_map<std::string, TokenType> Scanner::keywords = {
    {"and",    TokenType::AND},
    {"class",  TokenType::CLASS},
    {"else",   TokenType::ELSE},
    {"false",  TokenType::FALSE},
    {"for",    TokenType::FOR},
    {"fun",    TokenType::FUN},
    {"if",     TokenType::IF},
    {"nil",    TokenType::NIL},
    {"or",     TokenType::OR},
    {"print",  TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super",  TokenType::SUPER},
    {"this",   TokenType::THIS},
    {"true",   TokenType::TRUE},
    {"var",    TokenType::VAR},
    {"while",  TokenType::WHILE}
};

// 构造函数
Scanner::Scanner(const std::string& src) 
    : source(src) {
}

// 扫描所有 Token
std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    
    tokens.emplace_back(TokenType::EOF_TOKEN, "", std::any(), line);
    return tokens;
}

// 检查是否到达末尾
bool Scanner::isAtEnd() const {
    return current >= static_cast<int>(source.length());
}

// 匹配期望的字符
bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++;
    return true;
}

// 扫描单个 Token
void Scanner::scanToken() {
    char c = advance();
    
    switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
        
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        
        case '/':
            if (match('/')) {
                // 注释：跳到行尾
                while (peek() != '\n' && !isAtEnd()) {
                    advance();
                }
            }
            else if(match('*')){
                while(!(peek()=='*'&&peekNext()=='/')&&!isAtEnd()){
                    advance();
                }
                advance();
                advance();
            }
            else {
                addToken(TokenType::SLASH);
            }
            break;
        
        case ' ':
        case '\r':
        case '\t':
            // 忽略空白字符
            break;
        
        case '\n':
            line++;
            break;
        
        case '"':
            loxString();
            break;
        
        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                Lox::error(line, "Unexpected character.");
            }
            break;
    }
}

// 处理标识符
void Scanner::identifier() {
    while (isAlphaNumeric(peek())) {
        advance();
    }
    
    std::string text = source.substr(start, current - start);
    
    auto it = keywords.find(text);
    TokenType type = (it != keywords.end()) ? it->second : TokenType::IDENTIFIER;
    
    addToken(type);
}

// 判断是否是字母
bool Scanner::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

// 判断是否是字母或数字
bool Scanner::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

// 查看下一个字符
char Scanner::peekNext() const {
    if (current + 1 >= static_cast<int>(source.length())) {
        return '\0';
    }
    return source[current + 1];
}

// 处理数字
void Scanner::number() {
    while (isDigit(peek())) {
        advance();
    }
    
    // 查找小数部分
    if (peek() == '.' && isDigit(peekNext())) {
        // 消耗小数点
        advance();
        
        while (isDigit(peek())) {
            advance();
        }
    }
    
    std::string text = source.substr(start, current - start);
    double value = std::stod(text);
    addToken(TokenType::NUMBER, value);
}

// 判断是否是数字
bool Scanner::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

// 处理字符串
void Scanner::loxString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
        }
        advance();
    }
    
    if (isAtEnd()) {
        Lox::error(line, "Unterminated string.");
        return;
    }
    
    // 消耗结束的 "
    advance();
    
    // 去掉引号
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

// 查看当前字符
char Scanner::peek() const {
    if (isAtEnd()) {
        return '\0';
    }
    return source[current];
}

// 前进一个字符
char Scanner::advance() {
    return source[current++];
}

// 添加 Token（无字面量）
void Scanner::addToken(TokenType type) {
    addToken(type, std::any());
}

// 添加 Token（带字面量）
void Scanner::addToken(TokenType type, std::any literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
}

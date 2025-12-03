#include "RPNPrinter.h"
#include "TokenType.h"
#include <iostream>
using namespace std;
string RPNPrinter::print(Expr &expr) {
    return std::any_cast<string>(expr.accept(*this));
}

std::any RPNPrinter::visitBinaryExpr(Binary& expr) {
    return parenthesize(expr.op.lexeme, *expr.left, *expr.right);
}

std::any RPNPrinter::visitGroupingExpr(Grouping& expr) {
    return parenthesize("", *expr.expression);
}

// ✅ 修复：处理多种类型
std::any RPNPrinter::visitLiteralExpr(Literal& expr) {
    if (!expr.value.has_value()) {
        return std::string("nil");
    }
    return stringify(expr.value);
}

std::any RPNPrinter::visitUnaryExpr(Unary& expr) {
    return parenthesize(expr.op.lexeme, *expr.right);
}

std::string RPNPrinter::stringify(const std::any& value) {
    if (!value.has_value()) {
        return "nil";
    }
    
    // 尝试 int
    try {
        return std::to_string(std::any_cast<int>(value));
    } catch (const std::bad_any_cast&) {}
    
    // 尝试 double
    try {
        double d = std::any_cast<double>(value);
        std::string str = std::to_string(d);
        // 移除尾部的 0
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (!str.empty() && str.back() == '.') {
            str.pop_back();
        }
        return str;
    } catch (const std::bad_any_cast&) {}
    
    // 尝试 string
    try {
        return std::any_cast<std::string>(value);
    } catch (const std::bad_any_cast&) {}
    
    // 尝试 const char*
    try {
        return std::string(std::any_cast<const char*>(value));
    } catch (const std::bad_any_cast&) {}
    
    // 尝试 bool
    try {
        return std::any_cast<bool>(value) ? "true" : "false";
    } catch (const std::bad_any_cast&) {}
    
    return "unknown";
}


int main() {
    // 构建表达式: (- 123) * (45.67)
    auto expression = std::make_unique<Binary>(
        std::make_unique<Binary>(
            std::make_unique<Literal>(1),
            Token(TokenType::PLUS, "+", nullptr, 1),
            std::make_unique<Literal>(2)
        ),
        Token(TokenType::STAR, "*", nullptr, 1),
        std::make_unique<Binary>(
            std::make_unique<Literal>(4),
            Token(TokenType::MINUS, "-", nullptr, 1),
            std::make_unique<Literal>(3)
        )
    );
    
    // 使用 AstPrinter 打印
    RPNPrinter printer;
    std::string result = printer.print(*expression);
    std::cout << result << std::endl;
    
    return 0;
}


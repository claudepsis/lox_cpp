#include "AstPrinter.h"
#include "TokenType.h"
#include <iostream>
using namespace std;
string AstPrinter::print(Expr &expr) {
    return std::any_cast<string>(expr.accept(*this));
}

std::any AstPrinter::visitBinaryExpr(Binary& expr) {
    return parenthesize(expr.op.lexeme, *expr.left, *expr.right);
}

std::any AstPrinter::visitGroupingExpr(Grouping& expr) {
    return parenthesize("group", *expr.expression);
}

// ✅ 修复：处理多种类型
std::any AstPrinter::visitLiteralExpr(Literal& expr) {
    if (!expr.value.has_value()) {
        return std::string("nil");
    }
    return stringify(expr.value);
}

std::any AstPrinter::visitUnaryExpr(Unary& expr) {
    return parenthesize(expr.op.lexeme, *expr.right);
}

std::string AstPrinter::stringify(const std::any& value) {
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
        std::string str = formatDouble(d);
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


// int main() {
//     // 构建表达式: (- 123) * (45.67)
//     auto expression = std::make_unique<Binary>(
//         std::make_unique<Unary>(
//             Token(TokenType::MINUS, "-", nullptr, 1),
//             std::make_unique<Literal>(123)
//         ),
//         Token(TokenType::STAR, "*", nullptr, 1),
//         std::make_unique<Grouping>(
//             std::make_unique<Literal>(45.67)
//         )
//     );
    
//     // 使用 AstPrinter 打印
//     AstPrinter printer;
//     std::string result = printer.print(*expression);
//     std::cout << result << std::endl;
    
//     return 0;
// }





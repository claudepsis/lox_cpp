#include "Expr.h"
#include "Token.h"
class RPNPrinter:Visitor{


    std::any visitBinaryExpr(Binary& expr);

    std::any visitGroupingExpr(Grouping& expr);

    std::any visitLiteralExpr(Literal& expr);
    std::any visitUnaryExpr(Unary& expr);
    std::string stringify(const std::any& value);
    public:
    std::string print(Expr& expr);
    template<typename... Exprs>
    std::string parenthesize(const std::string &name,Exprs&...exprs){
        std::ostringstream oss;
        ((oss<<print(exprs)<<" "),...);
        oss<<name;
        return oss.str();
    }
};
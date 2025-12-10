#include "GenerateAst.h"
#include <iostream>
using namespace std;
typedef any Object;

std::string ltrim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    return (start == std::string::npos) ? "" : str.substr(start);
}

// 去除右边空白
std::string rtrim(const std::string& str) {
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

// 去除首尾空白
std::string trim(const std::string& str) {
    return rtrim(ltrim(str));
}

std::string toUpper(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return str;
}

// 转换为小写
std::string toLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return str;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    stringstream ss(str);
    std::string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

string convertVirtual(const string &name){
    if(name=="Expr") return "std::unique_ptr<"+name+">"; 
    return name;
}


static void defineType(ofstream& writer, 
                       const string& baseName,
                       const string& className, 
                       const string& fieldList) {
    writer << "class " << className << " : public " << baseName << " {\n";
    writer << "public:\n";
    
    // 解析字段
    vector<pair<string, string>> fields = parseFields(fieldList);
    
    // 成员变量声明
    for (const auto& [type, name] : fields) {  // C++17 结构化绑定
        writer << "    " << type << " " << name << ";\n";
    }
    writer << "\n";
    
    // 构造函数参数
    writer << "    " << className << "(";
    writer << join(fields, ", ", [](const auto& field) {
        return field.first + " " + field.second;
    });
    writer << ")\n";
    
    // 初始化列表
    writer << "        : ";
    writer << join(fields, ", ", [](const auto& field) {
        return field.second + "(std::move(" + field.second + "))";
    });
    writer << " {}\n";
    //init accept
    writer<<"\tstd::any accept(Visitor& visitor){"<<std::endl;
    writer<<"\t\treturn visitor.visit"<<className<<baseName<<"(*this);"<<endl;
    writer<<"\t}"<<endl;
    writer << "};\n\n";
}

// 辅助函数：解析字段列表
vector<pair<string, string>> parseFields(const string& fieldList) {
    vector<pair<string, string>> result;
    auto fields = split(fieldList, ',');
    
    for (const auto& field : fields) {
        auto parts = split(trim(field), ' ');
        if (parts.size() >= 2) {
            string type = convertVirtual(parts[0]);
            string name = parts[1];
            result.push_back({type, name});
        }
    }
    
    return result;
}

void defineVisitor(std::ofstream &writer,std::string baseName,std::vector<std::string>types){
    writer<<"class Visitor{"<<endl;
    writer<<"public:"<<endl;
    writer<<"\t"<<"virtual ~Visitor()=default;"<<endl;
    for(string type:types){
        string typeName=trim(split(type,':')[0]);
        writer<<"\tvirtual std::any visit"<<typeName<<baseName<<"("+typeName+"& "+toLower(baseName)+")=0;"<<endl;
    }
    writer<<"};\n"<<endl;
}

void defineAst(const string& outputDir,const string& baseName,const vector<string>&types){
    string path=outputDir+"/"+baseName+".h";
    ofstream writer(path);
    if(!writer.is_open()){
        cerr<<"Error: Cannot open file"<<path<<endl;
        return ;
    }
    writer<<"#ifndef "<<baseName<<"_H"<<endl;
    writer<<"#define "<<baseName<<"_H"<<endl;
    writer<<"#include<memory>"<<endl;
    writer<<"#include<any>"<<endl;
    writer<<"#include \"Token.h\""<<endl;
    writer<<"typedef std::any Object;"<<endl;
    for(const string& type:types){
        string className=trim(split(type,':')[0]);
        writer<<"class "<<className<<";"<<endl;  
    }
    writer<<"class "<<baseName<<"{"<<endl;
    writer<<"public:"<<endl;
    defineVisitor(writer,baseName,types);
    writer<<"\t"<<"virtual ~"<<baseName<<"()=default;"<<endl;
    writer<<"\t"<<"virtual std::any accept(Visitor& visitor)=0;"<<endl;
    writer<<"};\n"<<endl;
    
    for(const string& type:types){
        vector<string> parts=split(type,':');
        string filedList=trim(parts[1]);;
        defineType(writer,baseName,trim(parts[0]),filedList);  
    }
    writer<<"#endif"<<endl;
    writer.close();
}

int main(int argc,char*argv[]){
    if(argc!=2){
        cerr<<"Usage: genrate_ast <output directory>"<<std::endl;
        exit(64);
    }
    std::string outputDir=argv[1];
    defineAst(outputDir, "Expr", vector<string>{
        "Assign : Token name, Expr value",
      "Binary   : Expr left, Token op, Expr right",
      "Grouping : Expr expression",
      "Literal  : Object value",
      "Unary    : Token op, Expr right",
      "Variable : Token name",
      "Tenary   : Expr condtion, Expr thenBranch, Expr elseBranch"
    }); 
    defineAst(outputDir,"Stmt",vector<string>{
        "Expression : Expr expression",
        "Print  : Expr expression",
        "Var    :Token name,Expr initializer"
    });
}
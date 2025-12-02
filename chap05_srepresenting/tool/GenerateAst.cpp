#include <list>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;


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

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    stringstream ss(str);
    std::string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}


static void defineAst(const string& outputDir,const string& baseName,const vector<string>&types){
    string path=outputDir+"/"+baseName+".h";
    ofstream writer(path);
    if(!writer.is_open()){
        cerr<<"Error: Cannot open file"<<path<<endl;
        return ;
    }
    writer<<"#ifndef "<<baseName<<"_H"<<endl;
    writer<<"#define "<<baseName<<"_H"<<endl;
    
    writer<<"class Expr{"<<endl;
    writer<<"public:"<<endl;
    writer<<"\t"<<"virtual ~Expr()=default;"<<endl;
    writer<<"};"<<endl;
    writer<<"#endif"<<endl;
    for(const string& type:types){
        vector<string> parts=split(type,':');
        string className=trim(parts[0]);
        writer<<"class "<<className<<" : public "<<baseName<<" {"<<endl;
    }
}

int main(int argc,char*argv[]){
    if(argc!=2){
        cerr<<"Usage: genrate_ast <output directory>"<<std::endl;
        exit(64);
    }
    std::string outputDir=argv[1];
    defineAst(outputDir, "Expr", vector<string>{
      "Binary   : Expr left, Token operator, Expr right",
      "Grouping : Expr expression",
      "Literal  : Object value",
      "Unary    : Token operator, Expr right"
    }); 
}
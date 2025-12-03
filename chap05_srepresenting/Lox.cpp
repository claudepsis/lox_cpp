#include "Lox.h"
#include "Token.h"
#include "Scanner.h"
bool hadError=false;

void Lox::runFile(const std::string& path){
    std::ifstream file(path);
    if(!file.is_open()){
        throw std::runtime_error("could not open the file!"+path);
    }
    std::stringstream buffer;
    buffer<<file.rdbuf();
    std::string content=std::move(buffer.str());
    run(content);
    if(hadError) exit(65);
}

void Lox::runPrompt(){
    while(true){
        std::cout<<"> ";
        std::string line;
        std::cin>>line;
        if(line.empty()) break;
        run(line);
        hadError=false;
    }
}

void Lox::run(const std::string&source){
    Scanner scanner=Scanner(source);
    std::vector<Token> tokens=scanner.scanTokens();
    for(Token token:tokens){
        std::cout<<token.toString()<<std::endl;
    }
}

void Lox::error(int line,const std::string& message){
    report(line,"",message);
}


void Lox::report(int line,const std::string& where,const std::string& message){
    std::cerr<<"[line "<<line<<"] Error"<<where<<": "<<message<<std::endl;
    hadError=true;
}

// int main(int argc,char *argv[]){
//     if(argc>2){
//         std::cout<<"Usage:lox [script]";
//         exit(64);
//     }
//     else if(argc==2){
//         Lox::runFile(std::string(argv[1]));
//     }
//     else {
//         Lox::runPrompt();
//     }
// }


#ifndef GENERATE_AST_H
#define GENERATE_AST_H

#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <any>
#include <sstream>
#include <algorithm>
#include <cctype>

// 字符串处理函数
std::string ltrim(const std::string& str);
std::string rtrim(const std::string& str);
std::string trim(const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);
std::string toUpper(std::string str);
std::string toLower(std::string str) ;
// 类型转换函数
std::string convertVirtual(const std::string& name);

// 模板函数（需要在头文件中定义）
template<typename T, typename Func>
std::string join(const std::vector<T>& items, const std::string& separator, Func formatter) {
  std::string result;
  for (size_t i = 0; i < items.size(); i++) {
      result += formatter(items[i]);
      if (i < items.size() - 1) {
          result += separator;
      }
  }
  return result;
}


// 字段解析函数
std::vector<std::pair<std::string, std::string>> parseFields(const std::string& fieldList);

// AST生成函数
static void defineType(std::ofstream& writer, 
              const std::string& baseName,
              const std::string& className, 
              const std::string& fieldList);

static void defineAst(const std::string& outputDir,
             const std::string& baseName,
             const std::vector<std::string>& types);

static void defineVisitor(std::ofstream &writer,std::string baseName,std::vector<std::string>types);

#endif
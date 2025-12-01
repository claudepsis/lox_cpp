cmake_minimum_required(VERSION 3.10)
project(Lox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# ✅ 设置为 Debug 模式
set(CMAKE_BUILD_TYPE Debug)

# 或者手动添加调试标志
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
include_directories(include)

add_executable(lox
    Lox.cpp
    Scanner.cpp
    Token.cpp
)

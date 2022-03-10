cmake_minimum_required(VERSION 3.10)

project(SCC)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra)

include_directories(include)
add_executable(SCC
        src/main.cpp
        src/config.cpp
        src/log.cpp
        src/lexical_analyzer.cpp
        src/syntax_analyzer.cpp
        src/ast_handler.cpp
        src/query_assembler.cpp)

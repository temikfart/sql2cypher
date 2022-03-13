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


include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TestExec SCC_Test_System)
include_directories(include)
add_executable(${TestExec}
        test/config_test/config_test.cpp src/config.cpp src/log.cpp)

target_link_libraries(
        ${TestExec}
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(${TestExec})

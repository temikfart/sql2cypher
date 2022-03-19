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
        src/query_assembler.cpp
        src/ast.cpp)


include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        e2239ee6043f73722e7aa812a459f54a28552929
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TestExec SCC_Test_System)
include_directories(include)
add_executable(${TestExec}
        test/config_test/config_test.cpp src/config.cpp src/log.cpp
        test/log_test/log_test.cpp src/log.cpp)

target_link_libraries(
        ${TestExec}
        gtest_main
        gmock
)

include(GoogleTest)
gtest_discover_tests(${TestExec})

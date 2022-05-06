cmake_minimum_required(VERSION 3.10)

project(SCC)
# Set Version
set(deb_SCC_VERSION_MAJOR 0)
set(deb_SCC_VERSION_MINOR 9)
set(deb_SCC_VERSION_PATCH 0)
set(deb_SCC_VERSION "${deb_SCC_VERSION_MAJOR}.${deb_SCC_VERSION_MINOR}.${deb_SCC_VERSION_PATCH}")

set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    message("Are you using MacOS?")
elseif (WIN32)
    message("Are you using WIN32?")
else ()
    message("Are you using Linux?")

    add_compile_options(-Wall -Wextra)

    include_directories(include)
    add_executable(SCC
            src/main.cpp
            src/log.cpp
            src/config.cpp
            src/ast.cpp
            src/lexical_analyzer.cpp
            src/syntax_analyzer_general.cpp
            src/syntax_analyzer_ddl.cpp
            src/syntax_analyzer_dml.cpp
            src/syntax_analyzer_other.cpp
            src/ast_handler.cpp
            src/query_assembler_general.cpp
            src/query_assembler_ddl.cpp
            src/query_assembler_dml.cpp
            src/query_assembler_other.cpp
            src/tree_dump.cpp)

    # Binaries
    install (TARGETS SCC
            DESTINATION bin)

    # Library
    # Note: may not work on windows
    install (TARGETS SCC
            LIBRARY DESTINATION lib)

    # Config
    #    install (FILES cmake-examples.conf
    #            DESTINATION etc)

    # Tell CPack to generate a .deb package
    set(CPACK_GENERATOR "DEB")

    # Set a Package Maintainer.
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Artyom Fartygin, Roman Korostinskiy")

    # Set a brief description
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SQL to CypherQL Converter")

    # Set a long description
    set(CPACK_PACKAGE_DESCRIPTION "The SCC is a tool, which allows you \
    to migrate data from the relational (MS SQL) \
    to the graph (neo4j) database.")

    # Set a Package Version
    set(CPACK_PACKAGE_VERSION ${deb_SCC_VERSION})

    # Include CPack
    include(CPack)
endif ()

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TestExec SCC_Test_System)
include_directories(include)
add_executable(${TestExec}
        test/config_test/config_test.cpp src/config.cpp
        test/log_test/log_test.cpp src/log.cpp
        test/ast_test/ast_test.cpp src/ast.cpp
        test/lexical_analyzer_test/lexical_analyzer_test.cpp src/lexical_analyzer.cpp)

target_link_libraries(
        ${TestExec}
        gtest_main
        gmock
)

include(GoogleTest)
gtest_discover_tests(${TestExec})

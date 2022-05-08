cmake_minimum_required(VERSION 3.10)

# Set project
project(SCC)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set Version (rc - Release Candidate)
set(deb_SCC_VERSION_MAJOR 0)
set(deb_SCC_VERSION_MINOR 9)
set(deb_SCC_VERSION_PATCH 0)
set(deb_SCC_VERSION "${deb_SCC_VERSION_MAJOR}.${deb_SCC_VERSION_MINOR}.${deb_SCC_VERSION_PATCH}-rc")

# Set developers
set(deb_SCC_MAINTAINERS "Artyom Fartygin, Roman Korostinskiy")

# Cross-compilation
if (APPLE)
    message("Are you using ${CMAKE_SYSTEM_NAME}?")
elseif (WIN32)
    message("Are you using ${CMAKE_SYSTEM_NAME}?")
elseif (UNIX)
    message("Are you using ${CMAKE_SYSTEM_NAME}?")

    # Set compilation flags
    if (PACKAGE)
        message("Invoked the creation of a .deb package")
        set(PACKAGE_CREATION_FLAG "-DCREATE_DEBIAN_PACKAGE")
    endif ()
    set(MACROS_FLAGS "-Ddeb_SCC_VERSION=\"\\\"${deb_SCC_VERSION}\\\"\" -Ddeb_SCC_MAINTAINERS=\"\\\"${deb_SCC_MAINTAINERS}\\\"\" ${PACKAGE_CREATION_FLAG}")
    set(COMPILATION_FLAGS "-Wall -Wextra -g ${MACROS_FLAGS}")

    # Set C++ compiler's flags
    set(CMAKE_CXX_FLAGS ${COMPILATION_FLAGS})

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
    install(TARGETS SCC
            DESTINATION bin)

    # Create directory for the logs
    # /mnt/s/OneDrive\ -\ МФТИ/inf/4sem_proj/sql2cypher/SCC_log/
    install(DIRECTORY DESTINATION "/var/log/SCC_log/")

    # Tell CPack to generate a .deb package
    set(CPACK_GENERATOR "DEB")

    # Set a Package Maintainer.
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${deb_SCC_MAINTAINERS})

    # Set a brief description
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "SQL to CypherQL Converter")

    # Set a long description
    set(CPACK_PACKAGE_DESCRIPTION "The SCC is a tool, which allows you \
to migrate data from the relational (MS SQL) \
to the graph (neo4j) database.")

    # Set a Package Version
    set(CPACK_PACKAGE_VERSION ${deb_SCC_VERSION})

    # Set dependencies
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "graphviz (>= 2.42)")

    # Set home page
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/temikfart/sql2cypher")

    # Include CPack
    include(CPack)
else ()
    message("You are using unsupported by SCC system (${CMAKE_SYSTEM_NAME})")
endif ()

# Google Tests
if (NOT PACKAGE)
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
endif()

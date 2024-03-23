cmake_minimum_required(VERSION 3.16)

########################### COMPILER'S CONFIGURATION ###########################

# Set compilers' variables
set(IS_COMPILERS_SET false)
if (APPLE)
    message("Set C/C++ compilers for MacOS")

    # Set compilers for MacOS
#    set(C_COMPILER "")
#    set(CXX_COMPILER "")
#    set(IS_COMPILERS_SET true)

    # Set compilation flag (package)
    set(PACKAGE_TYPE ".app")
    set(PACKAGE_TYPE_FLAG "-DAPP_PACKAGE")
elseif (WIN32)
    message("Set C/C++ compilers for Windows")

    # Set compilers for Windows
#    set(C_COMPILER "C:/MinGW/bin/gcc")
#    set(CXX_COMPILER "C:/MinGW/bin/g++")
#    set(IS_COMPILERS_SET true)

    # Set compilation flag (package)
    set(PACKAGE_TYPE ".exe")
    set(PACKAGE_TYPE_FLAG "-DEXE_PACKAGE")
elseif (UNIX)
    message("Set C/C++ compilers for Linux")

    # Set compilers for Linux
    set(C_COMPILER "/usr/bin/gcc")
    set(CXX_COMPILER "/usr/bin/g++")
    set(IS_COMPILERS_SET true)

    # Set compilation flag (package)
    set(PACKAGE_TYPE ".deb")
    set(PACKAGE_TYPE_FLAG "-DDEBIAN_PACKAGE")
else ()
    message(SEND_ERROR "Unsupported system")
endif ()

# Set compilers
if (IS_COMPILERS_SET)
    set(CMAKE_C_COMPILER ${C_COMPILER})
    set(CMAKE_CXX_COMPILER ${CXX_COMPILER})
endif ()

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)


############################# PROJECT INFORMATION ##############################

# Set project name
set(PRJ_NAME SCC)
project(${PRJ_NAME})

# Print current system
message("Current system is ${CMAKE_SYSTEM_NAME}")

# Set Version
set(PRJ_VERSION_MAJOR 0)
set(PRJ_VERSION_MINOR 9)
set(PRJ_VERSION_PATCH 1)
set(PRJ_VERSION "${PRJ_VERSION_MAJOR}\
.${PRJ_VERSION_MINOR}\
.${PRJ_VERSION_PATCH}")

# Set developers
set(PRJ_MAINTAINERS "Artyom Fartygin, Roman Korostinskiy")


########################## CROSS-PLATFORM COMPILATION ##########################

# If package is created, then set macros
if (PACKAGE)
    message("Invoked a creation of the \"${PACKAGE_TYPE}\" package")
    set(MACROS_PACKAGE_FLAGS "-DCREATE_PACKAGE ${PACKAGE_TYPE_FLAG}")
endif ()

# Set compilation flags
set(MACROS_FLAGS "-DSCC_VERSION=\"\\\"${PRJ_VERSION}\\\"\" \
-DSCC_MAINTAINERS=\"\\\"${PRJ_MAINTAINERS}\\\"\" ${MACROS_PACKAGE_FLAGS}")
set(COMPILATION_FLAGS "-Wall -Wextra -g ${MACROS_FLAGS}")
set(CMAKE_CXX_FLAGS ${COMPILATION_FLAGS})

# Create executable or package
include_directories(include)
set(PRJ_TARGET scc)

set(COMMON_SRC
        src/common/file_utils.cpp
        src/common/singleton.cpp
        src/common/string_utils.cpp
)
set(LOG_SRC
        src/log.cpp
)
set(CONFIG_SRC
        src/config/scc_args.cpp
        src/config/scc_config.cpp
        src/config/scc_mode.cpp
)
set(AST_SRC
        src/ast/nodes/inode.cpp
        src/ast/nodes/char_node.cpp
        src/ast/nodes/float_node.cpp
        src/ast/nodes/int_node.cpp
        src/ast/nodes/root_node.cpp
        src/ast/nodes/service_node.cpp
        src/ast/nodes/string_node.cpp
        src/ast/data_types.cpp
        src/ast/stmt_types.cpp
)
set(SOURCE_LIST src/main.cpp
        ${COMMON_SRC}
        ${LOG_SRC}
        ${CONFIG_SRC}
        ${AST_SRC}
        src/lexer/lexical_analyzer.cpp
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

include(FetchContent)
FetchContent_Declare(
        logger
        GIT_REPOSITORY https://github.com/temikfart/logger.git
        GIT_TAG main # latest
)
FetchContent_MakeAvailable(logger)
FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
        GIT_TAG 997da9255618311d1fcb0135ce86022729d1f1cb # release-2.9
)
FetchContent_MakeAvailable(argparse)

# Set information about an application
set(PRJ_SHORT_DESCRIPTION "SQL to CypherQL Converter")
set(PRJ_LONG_DESCRIPTION "The SCC is a tool, which allows you \
to migrate data from the relational (MS SQL) \
to the graph (neo4j) database.")
set(PRJ_HOMEPAGE "https://github.com/temikfart/sql2cypher")

if (APPLE)
    # Create bundle or executable
    if (PACKAGE)
        add_executable(${PRJ_TARGET} MACOSX_BUNDLE ${SOURCE_LIST})

        set_target_properties(${PRJ_TARGET} PROPERTIES
                BUNDLE True
                MACOSX_BUNDLE_LOCATION "MacOS"
                MACOSX_BUNDLE_BUNDLE_NAME ${PRJ_TARGET}
                MACOSX_BUNDLE_BUNDLE_VERSION "${PRJ_VERSION}"
                MACOSX_BUNDLE_SHORT_VERSION_STRING "${PRJ_VERSION}"
                )
    else ()
        add_executable(${PRJ_TARGET} ${SOURCE_LIST})
    endif ()

elseif (WIN32)
    add_executable(${PRJ_TARGET} ${SOURCE_LIST})

    # Create package with CPack
    if (PACKAGE)
        # Tell CPack to generate a .exe package
        set(CPACK_GENERATOR "NSIS")

        # Binaries
        install(TARGETS ${PRJ_TARGET}
                DESTINATION bin)

        # Create directory for the logs
        install(DIRECTORY DESTINATION log DIRECTORY_PERMISSIONS
                OWNER_WRITE OWNER_READ OWNER_EXECUTE
                GROUP_WRITE GROUP_READ GROUP_EXECUTE
                WORLD_WRITE WORLD_READ WORLD_EXECUTE)

            set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PRJ_NAME}")

        # Uninstall previous program before installing a new
        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

        # Set display name
        set(CPACK_NSIS_DISPLAY_NAME ${PRJ_SHORT_DESCRIPTION})

        # Set package name
        set(CPACK_NSIS_PACKAGE_NAME ${PRJ_SHORT_DESCRIPTION})

        # Set help link
        set(CPACK_NSIS_HELP_LINK ${PRJ_HOMEPAGE})
        set(CPACK_NSIS_URL_INFO_ABOUT ${PRJ_HOMEPAGE})

        # Set a Package Version
        set(CPACK_PACKAGE_VERSION ${PRJ_VERSION})

        # Set package name
        set(CPACK_PACKAGE_NAME "${PRJ_NAME}")

        # Set a brief description
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PRJ_SHORT_DESCRIPTION})

        # Set a long description
        set(CPACK_PACKAGE_DESCRIPTION ${PRJ_LONG_DESCRIPTION})

        # Include CPack
        include(CPack)
    endif()
elseif (UNIX)
    add_executable(${PRJ_TARGET} ${SOURCE_LIST})

    # Create package with CPack
    if (PACKAGE)
        # Tell CPack to generate a .deb package
        set(CPACK_GENERATOR "DEB")

        # Binaries
        install(TARGETS scc
                DESTINATION bin)

        # Create directory for the logs
        install(DIRECTORY DESTINATION "/var/log/${PRJ_NAME}_log/")

        # Set a Package Maintainer.
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${PRJ_MAINTAINERS})

        # Set a brief description
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PRJ_SHORT_DESCRIPTION})

        # Set a long description
        set(CPACK_PACKAGE_DESCRIPTION ${PRJ_LONG_DESCRIPTION})

        # Set a Package Version
        set(CPACK_PACKAGE_VERSION ${PRJ_VERSION})

        # Set dependencies
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "graphviz (>= 2.42)")

        # Set home page
        set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${PRJ_HOMEPAGE})

        # Include CPack
        include(CPack)
    endif()
endif ()

target_link_libraries(${PRJ_TARGET} PRIVATE liblogger argparse)


################################ ENABLE TESTING ################################

set(TEST_TARGET ${PRJ_TARGET}_test)

# Include Google Tests
if (NOT PACKAGE)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    include_directories(include)
    include_directories(test/fixtures/include)
    # Make sure, that source files are mentioned ONE time
    set(TEST_FIXTURES
            test/fixtures/src/scc_args_fixtures.cpp
            test/fixtures/src/scc_config_fixtures.cpp
    )
    set(AST_TEST
            test/ast/data_types_test.cpp
            src/ast/data_types.cpp

            test/ast/stmt_types_test.cpp
            src/ast/stmt_types.cpp
    )
    set(COMMON_TEST
            test/common/file_utils_test.cpp
            src/common/file_utils.cpp

            test/common/string_utils_test.cpp
            src/common/string_utils.cpp
    )
    set(CONFIG_TEST
            test/config/scc_args_test.cpp
            src/config/scc_args.cpp
            src/log.cpp

            test/config/scc_config_test.cpp
            src/config/scc_config.cpp

            test/config/scc_mode_test.cpp
            src/config/scc_mode.cpp
    )
    # Temporarily disabled
#    set(LOG_TEST
#            test/log_test/log_test.cpp
#            src/log.cpp)
#    set(AST_TEST
#            test/ast_test/ast_test.cpp
#            src/ast.cpp)
#    set(LEXICAL_ANALYZER_TEST
#            test/lexical_analyzer_test/lexical_analyzer_test.cpp
#            src/lexical_analyzer.cpp)
    # Broken tests
#    set(SQL_AST_TEST
#            test/SQL_AST_test/SQL_AST_test.cpp src/syntax_analyzer_general.cpp
#            src/syntax_analyzer_other.cpp src/syntax_analyzer_ddl.cpp src/syntax_analyzer_dml.cpp
#            src/tree_dump.cpp)
    add_executable(${TEST_TARGET} ${TEST_FIXTURES}
            ${COMMON_TEST}
            ${CONFIG_TEST}
            ${LOG_TEST}
            ${AST_TEST}
            ${LEXICAL_ANALYZER_TEST}
            ${SQL_AST_TEST}
    )

    target_link_libraries(
            ${TEST_TARGET}
            GTest::gtest_main
            GTest::gmock
            liblogger
            argparse
    )

    include(GoogleTest)
    gtest_discover_tests(${TEST_TARGET})
endif()

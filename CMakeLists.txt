cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    set(PACKAGE_TYPE ".app")
    set(PACKAGE_TYPE_FLAG "-DAPP_PACKAGE")
elseif (WIN32)
    set(PACKAGE_TYPE ".exe")
    set(PACKAGE_TYPE_FLAG "-DEXE_PACKAGE")
elseif (UNIX)
    set(PACKAGE_TYPE ".deb")
    set(PACKAGE_TYPE_FLAG "-DDEBIAN_PACKAGE")
else ()
    message(SEND_ERROR "Unsupported system")
endif ()
message(DEBUG "System name: ${CMAKE_SYSTEM_NAME}")

# Project Description

set(PRJ_NAME SCC)
project(${PRJ_NAME})

set(PRJ_VERSION_MAJOR 0)
set(PRJ_VERSION_MINOR 9)
set(PRJ_VERSION_PATCH 1)
set(PRJ_VERSION "${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH}")

set(PRJ_SHORT_DESCRIPTION "SQL to Cypher Converter")
set(PRJ_LONG_DESCRIPTION "The SCC is a tool, which allows you \
to migrate schema from the relational (MS SQL Server) \
to the graph (Neo4j) database.")
set(PRJ_HOMEPAGE "https://github.com/temikfart/sql2cypher")
set(PRJ_MAINTAINERS "Artyom Fartygin (fartygin.a@phystech.edu),\
Roman Korostinskiy (korostinkii.r@phystech.edu)")

# Compilation

if (PACKAGE)
    message("Invoked a creation of the \"${PACKAGE_TYPE}\" package")
    set(MACROS_PACKAGE_FLAGS "-DCREATE_PACKAGE ${PACKAGE_TYPE_FLAG}")
endif ()

if (NOT WIN32)
    set(MACROS_FLAGS "-DSCC_VERSION=\"\\\"${PRJ_VERSION}\\\"\" \
    -DSCC_MAINTAINERS=\"\\\"${PRJ_MAINTAINERS}\\\"\" ${MACROS_PACKAGE_FLAGS}")
    set(COMPILATION_FLAGS "-Wall -Wextra -g ${MACROS_FLAGS}")
    if (COVERAGE)
        set(COMPILATION_FLAGS "${COMPILATION_FLAGS} --coverage")
    endif()
    set(CMAKE_C_FLAGS ${COMPILATION_FLAGS})
    if (EXPERIMENTAL)
        message("Adding -fexperimental-library flag to C++ compilation")
        set(COMPILATION_FLAGS "${COMPILATION_FLAGS} -fexperimental-library")
    endif()
    set(CMAKE_CXX_FLAGS ${COMPILATION_FLAGS})
endif()

## Download dependencies

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

## Build sources

add_library(scc_include INTERFACE)
target_include_directories(scc_include INTERFACE
        include
        ${logger_SOURCE_DIR}/include
        ${argparse_SOURCE_DIR}/include
)
set(SCC_INCLUDE_LIBS scc_include)

add_subdirectory(src)
set(SCC_SOURCE_LIBS
        scc_ast
        scc_common
        scc_config
        scc_dump
        scc_lexer
        scc_log
        scc_parser
        scc_translator
)

## Create executable

set(PRJ_TARGET scc)
add_executable(${PRJ_TARGET} src/main.cpp)
target_link_libraries(${PRJ_TARGET} PUBLIC
        ${SCC_INCLUDE_LIBS}
        ${SCC_SOURCE_LIBS}
)
target_link_libraries(${PRJ_TARGET} PUBLIC
        liblogger
        argparse
)

# Tests

enable_testing()
add_subdirectory(test)

# Packaging

if (PACKAGE)
    include(cmake/packaging.cmake)
    if (APPLE)
        make_darwin_package()
    elseif (WIN32)
        make_win_package()
    elseif (UNIX)
        make_unix_package()
    endif ()
endif ()

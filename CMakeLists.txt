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

# Cross-platform compilation

if (PACKAGE)
    message("Invoked a creation of the \"${PACKAGE_TYPE}\" package")
    set(MACROS_PACKAGE_FLAGS "-DCREATE_PACKAGE ${PACKAGE_TYPE_FLAG}")
endif ()

set(MACROS_FLAGS "-DSCC_VERSION=\"\\\"${PRJ_VERSION}\\\"\" \
-DSCC_MAINTAINERS=\"\\\"${PRJ_MAINTAINERS}\\\"\" ${MACROS_PACKAGE_FLAGS}")
set(COMPILATION_FLAGS "-Wall -Wextra -g ${MACROS_FLAGS}")
set(CMAKE_CXX_FLAGS ${COMPILATION_FLAGS})

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
        scc_query_assembler
)

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

#if (APPLE)
#    # Create bundle or executable
#    if (PACKAGE)
#        add_executable(${PRJ_TARGET} MACOSX_BUNDLE ${SOURCE_LIST})
#
#        set_target_properties(${PRJ_TARGET} PROPERTIES
#                BUNDLE True
#                MACOSX_BUNDLE_LOCATION "MacOS"
#                MACOSX_BUNDLE_BUNDLE_NAME ${PRJ_TARGET}
#                MACOSX_BUNDLE_BUNDLE_VERSION "${PRJ_VERSION}"
#                MACOSX_BUNDLE_SHORT_VERSION_STRING "${PRJ_VERSION}"
#                )
#    else ()
#        add_executable(${PRJ_TARGET} ${SOURCE_LIST})
#    endif ()
#
#elseif (WIN32)
#    add_executable(${PRJ_TARGET} ${SOURCE_LIST})
#
#    # Create package with CPack
#    if (PACKAGE)
#        # Tell CPack to generate a .exe package
#        set(CPACK_GENERATOR "NSIS")
#
#        # Binaries
#        install(TARGETS ${PRJ_TARGET}
#                DESTINATION bin)
#
#        # Create directory for the logs
#        install(DIRECTORY DESTINATION log DIRECTORY_PERMISSIONS
#                OWNER_WRITE OWNER_READ OWNER_EXECUTE
#                GROUP_WRITE GROUP_READ GROUP_EXECUTE
#                WORLD_WRITE WORLD_READ WORLD_EXECUTE)
#
#            set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PRJ_NAME}")
#
#        # Uninstall previous program before installing a new
#        set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
#
#        # Set display name
#        set(CPACK_NSIS_DISPLAY_NAME ${PRJ_SHORT_DESCRIPTION})
#
#        # Set package name
#        set(CPACK_NSIS_PACKAGE_NAME ${PRJ_SHORT_DESCRIPTION})
#
#        # Set help link
#        set(CPACK_NSIS_HELP_LINK ${PRJ_HOMEPAGE})
#        set(CPACK_NSIS_URL_INFO_ABOUT ${PRJ_HOMEPAGE})
#
#        # Set a Package Version
#        set(CPACK_PACKAGE_VERSION ${PRJ_VERSION})
#
#        # Set package name
#        set(CPACK_PACKAGE_NAME "${PRJ_NAME}")
#
#        # Set a brief description
#        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PRJ_SHORT_DESCRIPTION})
#
#        # Set a long description
#        set(CPACK_PACKAGE_DESCRIPTION ${PRJ_LONG_DESCRIPTION})
#
#        # Include CPack
#        include(CPack)
#    endif()
#elseif (UNIX)
#    add_executable(${PRJ_TARGET} ${SOURCE_LIST})
#
#    # Create package with CPack
#    if (PACKAGE)
#        # Tell CPack to generate a .deb package
#        set(CPACK_GENERATOR "DEB")
#
#        # Binaries
#        install(TARGETS scc
#                DESTINATION bin)
#
#        # Create directory for the logs
#        install(DIRECTORY DESTINATION "/var/log/${PRJ_NAME}_log/")
#
#        # Set a Package Maintainer.
#        set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${PRJ_MAINTAINERS})
#
#        # Set a brief description
#        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PRJ_SHORT_DESCRIPTION})
#
#        # Set a long description
#        set(CPACK_PACKAGE_DESCRIPTION ${PRJ_LONG_DESCRIPTION})
#
#        # Set a Package Version
#        set(CPACK_PACKAGE_VERSION ${PRJ_VERSION})
#
#        # Set dependencies
#        set(CPACK_DEBIAN_PACKAGE_DEPENDS "graphviz (>= 2.42)")
#
#        # Set home page
#        set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${PRJ_HOMEPAGE})
#
#        # Include CPack
#        include(CPack)
#    endif()
#endif ()

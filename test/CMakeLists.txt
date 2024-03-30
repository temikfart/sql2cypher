FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(scc_test_libs INTERFACE)
target_link_libraries(scc_test_libs INTERFACE
        ${SCC_INCLUDE_LIBS}
        ${SCC_SOURCE_LIBS}
        gmock_main
        gmock
        gtest
)

function(scc_test TARGET_NAME)
    add_executable(${TARGET_NAME} ${ARGN})
    target_link_libraries(${TARGET_NAME} PUBLIC scc_test_libs)
    include(GoogleTest)
    gtest_discover_tests(${TARGET_NAME})
endfunction(scc_test)

set(DIRS
        ast
        common
#        config
#        fixtures
        lexer
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

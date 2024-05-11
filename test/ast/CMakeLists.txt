set(DIRS
        common
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

scc_test(scc_test_ast_types
        data_types_test.cpp
        stmt_types_test.cpp
)

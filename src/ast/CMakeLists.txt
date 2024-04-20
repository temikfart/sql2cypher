set(DIRS
        common
        nodes
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

add_library(scc_ast STATIC)
target_sources(scc_ast PRIVATE
        data_types.cpp
        stmt_types.cpp
)
target_link_libraries(scc_ast PRIVATE
        scc_include
        scc_common
)
target_link_libraries(scc_ast PUBLIC
        scc_ast_nodes
        scc_ast_common
)

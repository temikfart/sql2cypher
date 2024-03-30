add_subdirectory(nodes)

add_library(scc_ast STATIC)
target_sources(scc_ast PRIVATE
        data_types.cpp
        stmt_types.cpp
)
target_link_libraries(scc_ast PRIVATE scc_include)
target_link_libraries(scc_ast PUBLIC scc_ast_nodes)

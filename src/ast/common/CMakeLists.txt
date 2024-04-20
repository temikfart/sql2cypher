add_library(scc_ast_common STATIC)
target_sources(scc_ast_common PRIVATE
        utils.cpp
)
target_link_libraries(scc_ast_common PRIVATE scc_include)

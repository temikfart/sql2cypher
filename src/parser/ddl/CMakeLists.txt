add_library(scc_parser_ddl STATIC)
target_sources(scc_parser_ddl PRIVATE
        ddl_statements.cpp
)
target_link_libraries(scc_parser_ddl PRIVATE
        scc_include
        scc_ast
        scc_parser_common
)

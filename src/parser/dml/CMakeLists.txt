add_library(scc_parser_dml STATIC)
target_sources(scc_parser_dml PRIVATE
        conditions.cpp
        dml_statements.cpp
        math_expressions.cpp
)
target_link_libraries(scc_parser_dml PRIVATE
        scc_include
        scc_ast
        scc_parser_common
)

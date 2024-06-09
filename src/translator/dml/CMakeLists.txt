add_library(scc_translator_dml STATIC)
target_sources(scc_translator_dml PRIVATE
        conditions.cpp
        dml_statements.cpp
        math_expressions.cpp
)
target_link_libraries(scc_translator_dml PRIVATE
        scc_include
        scc_ast
        scc_translator_common
        scc_translator_cypher_clauses
        scc_translator_cypher_graph
)

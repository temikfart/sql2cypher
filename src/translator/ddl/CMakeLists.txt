add_library(scc_translator_ddl STATIC)
target_sources(scc_translator_ddl PRIVATE
        ddl_statements.cpp
)
target_link_libraries(scc_translator_ddl PRIVATE
        scc_include
        scc_ast
        scc_translator_common
        scc_translator_cypher_clauses
        scc_translator_cypher_graph
)

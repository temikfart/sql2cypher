add_library(scc_translator_common STATIC)
target_sources(scc_translator_common PRIVATE
        basic_statements.cpp
        validation.cpp
)
target_link_libraries(scc_translator_common PRIVATE
        scc_include
        scc_ast
        scc_translator_cypher_clauses
        scc_translator_cypher_graph
)

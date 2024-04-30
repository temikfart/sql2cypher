add_library(scc_translator_cypher_clauses STATIC)
target_sources(scc_translator_cypher_clauses PRIVATE
        create.cpp
)
target_link_libraries(scc_translator_cypher_clauses PRIVATE
        scc_include
)

set(DIRS
        clauses
        graph
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

add_library(scc_translator_cypher STATIC)
target_sources(scc_translator_cypher PRIVATE
        schema.cpp
)
target_link_libraries(scc_translator_cypher PRIVATE
        scc_include
)
target_link_libraries(scc_translator_cypher PUBLIC
        scc_translator_cypher_clauses
        scc_translator_cypher_graph
)

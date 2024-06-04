set(DIRS
        common
        cypher
        ddl
        dml
        schema
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

add_library(scc_translator STATIC)
target_sources(scc_translator PRIVATE
        translator.cpp
)
target_link_libraries(scc_translator PRIVATE scc_include)
target_link_libraries(scc_translator PUBLIC
        scc_translator_common
        scc_translator_cypher_clauses
        scc_translator_cypher_graph
        scc_translator_ddl
        scc_translator_dml
        scc_translator_schema
)

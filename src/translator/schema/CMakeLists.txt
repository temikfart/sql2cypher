add_library(scc_translator_schema STATIC)
target_sources(scc_translator_schema PRIVATE
        node.cpp
        property.cpp
        relationship.cpp
        schema.cpp
)
target_link_libraries(scc_translator_schema PRIVATE
        scc_include
        scc_translator_cypher_graph
)

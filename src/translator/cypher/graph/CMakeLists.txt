add_library(scc_translator_cypher_graph STATIC)
target_sources(scc_translator_cypher_graph PRIVATE
        constraint_types.cpp
        node.cpp
        property.cpp
        property_types.cpp
        relationship.cpp
)
target_link_libraries(scc_translator_cypher_graph PRIVATE
        scc_include
        scc_common
)

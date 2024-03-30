add_library(scc_ast_nodes STATIC)
target_sources(scc_ast_nodes PRIVATE
        char_node.cpp
        float_node.cpp
        inode.cpp
        int_node.cpp
        root_node.cpp
        service_node.cpp
        string_node.cpp
)
target_link_libraries(scc_ast_nodes PRIVATE scc_include)

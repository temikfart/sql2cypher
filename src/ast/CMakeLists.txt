add_library(scc_ast STATIC)
target_sources(scc_ast PRIVATE
        data_types.cpp
        stmt_types.cpp

        nodes/char_node.cpp
        nodes/float_node.cpp
        nodes/inode.cpp
        nodes/int_node.cpp
        nodes/root_node.cpp
        nodes/service_node.cpp
        nodes/string_node.cpp
)
target_link_libraries(scc_ast PRIVATE scc_include)

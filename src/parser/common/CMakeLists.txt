add_library(scc_parser_common STATIC)
target_sources(scc_parser_common PRIVATE
        base_stmttype_classifier.cpp
        basic_statements.cpp
        node_data_classifier.cpp
        node_datatype_classifier.cpp
        validation.cpp
)
target_link_libraries(scc_parser_common PRIVATE
        scc_include
        scc_ast
)

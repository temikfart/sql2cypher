add_library(scc_parser STATIC)
target_sources(scc_parser PRIVATE
        parser_ddl.cpp
        parser_dml.cpp
        parser_general.cpp
        parser_other.cpp
        utils.cpp
)
target_link_libraries(scc_parser PRIVATE
        scc_include
        scc_ast
)

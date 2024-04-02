add_library(scc_parser STATIC)
target_sources(scc_parser PRIVATE
        syntax_analyzer_ddl.cpp
        syntax_analyzer_dml.cpp
        syntax_analyzer_general.cpp
        syntax_analyzer_other.cpp
        syntax_analyzer_utils.cpp
)
target_link_libraries(scc_parser PRIVATE
        scc_include
        scc_ast
)

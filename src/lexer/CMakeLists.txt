add_library(scc_lexer STATIC)
target_sources(scc_lexer PRIVATE
        lexer.cpp
        symbol_classifier.cpp
        symbol_types.cpp
)
target_link_libraries(scc_lexer PRIVATE
        scc_include
        scc_ast
)

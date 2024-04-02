set(DIRS
        common
        ddl
        dml
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

add_library(scc_parser STATIC)
target_sources(scc_parser PRIVATE
        parser.cpp
)
target_link_libraries(scc_parser PRIVATE
        scc_include
        scc_ast
)
target_link_libraries(scc_parser PUBLIC
        scc_parser_ddl
        scc_parser_dml
        scc_parser_common
)

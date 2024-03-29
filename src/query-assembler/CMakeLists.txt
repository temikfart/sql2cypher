add_library(scc_query_assembler STATIC)
target_sources(scc_query_assembler PRIVATE
        query_assembler_ddl.cpp
        query_assembler_dml.cpp
        query_assembler_general.cpp
        query_assembler_other.cpp
)
target_link_libraries(scc_query_assembler PRIVATE scc_include)

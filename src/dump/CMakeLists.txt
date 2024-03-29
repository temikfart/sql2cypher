add_library(scc_dump STATIC)
target_sources(scc_dump PRIVATE
        tree_dump.cpp
)
target_link_libraries(scc_dump PRIVATE scc_include)

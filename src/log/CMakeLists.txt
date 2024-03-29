add_library(scc_log STATIC)
target_sources(scc_log PRIVATE
        log.cpp
)
target_link_libraries(scc_log PRIVATE scc_include)

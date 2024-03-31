add_library(scc_config STATIC)
target_sources(scc_config PRIVATE
        scc_args.cpp
        scc_config.cpp
        scc_mode.cpp
)
target_link_libraries(scc_config PRIVATE scc_include)

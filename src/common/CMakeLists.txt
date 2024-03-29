add_library(scc_common STATIC)
target_sources(scc_common PRIVATE
        file_utils.cpp
        singleton.cpp
        string_utils.cpp
)
target_link_libraries(scc_common PRIVATE scc_include)

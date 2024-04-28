set(DIRS
        common
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

add_library(scc_translator STATIC)
target_sources(scc_translator PRIVATE
        translator_ddl.cpp
        translator_dml.cpp
        translator_general.cpp
        translator_other.cpp
)
target_link_libraries(scc_translator PRIVATE scc_include)
target_link_libraries(scc_translator PUBLIC
        scc_translator_common
)

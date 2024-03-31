add_library(scc_test_fixtures_include INTERFACE)
target_include_directories(scc_test_fixtures_include INTERFACE
        include
)
target_link_libraries(scc_test_fixtures_include INTERFACE
        scc_test_libs
)

add_subdirectory(src)

add_library(scc_test_fixtures INTERFACE)
target_link_libraries(scc_test_fixtures INTERFACE
        scc_test_fixtures_include
        scc_test_fixtures_config
)

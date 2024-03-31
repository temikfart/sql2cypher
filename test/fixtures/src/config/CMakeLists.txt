add_library(scc_test_fixtures_config STATIC)
target_sources(scc_test_fixtures_config PRIVATE
        scc_args_fixtures.cpp
        scc_config_fixtures.cpp
)
target_link_libraries(scc_test_fixtures_config PRIVATE scc_test_fixtures_include)

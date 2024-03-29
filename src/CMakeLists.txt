set(DIRS
        ast
        common
        config
        dump
        lexer
        log
        parser
        query-assembler
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

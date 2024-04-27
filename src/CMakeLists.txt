set(DIRS
        ast
        common
        config
        dump
        lexer
        log
        parser
        translator
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

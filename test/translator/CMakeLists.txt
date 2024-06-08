set(DIRS
        cypher
        schema
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

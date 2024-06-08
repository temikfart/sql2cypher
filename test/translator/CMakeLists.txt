set(DIRS
        cypher
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

set(DIRS
        clauses
        graph
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

set(DIRS
        graph
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

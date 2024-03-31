set(DIRS
        config
)
foreach(DIR ${DIRS})
    add_subdirectory(${DIR})
endforeach()

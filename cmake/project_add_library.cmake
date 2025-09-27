function(project_add_library)
    add_library(${ARGV})

    project_compile_lib()

    if(BUILD_TESTING AND EXISTS test)
        add_subdirectory(test)
    endif()
endfunction()

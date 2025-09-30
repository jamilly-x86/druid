function(project_add_library)
    add_library(${ARGV})

    project_compile_lib()

    # If the library has a `test` subdirectory, add it with an explicit
    # binary-dir so test CMake files are generated in the library build tree.
    if(BUILD_TESTING AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_CURRENT_BINARY_DIR}/test)
    endif()
endfunction()

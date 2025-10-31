function(project_add_library target)
    add_library(${target})

    get_property(PROJECT_LIB_TARGETS GLOBAL PROPERTY PROJECT_LIB_TARGETS)
    list(APPEND PROJECT_LIB_TARGETS ${target})
    set_property(GLOBAL PROPERTY PROJECT_LIB_TARGETS ${PROJECT_LIB_TARGETS})

    project_compile_lib()

    # If the library has a `test` subdirectory, add it with an explicit
    # binary-dir so test CMake files are generated in the library build tree.
    if(BUILD_TESTING AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_CURRENT_BINARY_DIR}/test)
    endif()
endfunction()

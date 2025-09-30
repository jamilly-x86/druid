function(project_add_executable target)
    add_executable(${target})

    project_compile_exe()

    install(TARGETS ${target}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    # If a `test` subdirectory exists in the component's source tree, add it.
    # Use an absolute path to the source and a matching binary-dir so this works
    # regardless of where this function was defined or executed.
    if(BUILD_TESTING AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_CURRENT_BINARY_DIR}/test)
    endif()

    # Make it possible to easily debug applications through the visual studio debugger.
    # Make it possible to easily debug applications through the Visual Studio debugger.
    if(MSVC)
        set(PROJECT_VCPKG_ROOT ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
        # Use the actual target name passed into the function, not ${PROJECT_NAME}.
        set_target_properties(${target} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${PROJECT_VCPKG_ROOT}/bin")
    endif()
endfunction()

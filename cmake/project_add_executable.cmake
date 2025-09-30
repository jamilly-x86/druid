function(project_add_executable target)
    add_executable(${target})

    project_compile_exe()

    install(TARGETS ${target}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

    if(BUILD_TESTING AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
        add_subdirectory(test)
    endif()

    # Make it possible to easily debug applications through the visual studio debugger.
    if(MSVC)
        set(PROJECT_VCPKG_ROOT ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET})
        set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${PROJECT_VCPKG_ROOT}/bin")
    endif()
endfunction()

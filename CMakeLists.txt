cmake_minimum_required(VERSION 3.31)

include(FetchContent)

# Acquire VCPKG
FetchContent_Declare(
    vcpkg
    GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
    GIT_TAG 2025.09.17
)

FetchContent_MakeAvailable(vcpkg)

# This needs to be on to match the gnu extensions used to compile the std.pcm
# on ubuntu 25.10 for import std; to work with clang.
# set(CMAKE_CXX_EXTENSIONS ON)
# import std; isn't quite read for clang's libc++ on Apple.
# set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
# set(CMAKE_CXX_MODULE_STD ON)

project(druid LANGUAGES CXX)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(cmake/project_compile_options.cmake)
include(cmake/project_add_library.cmake)
include(cmake/project_add_executable.cmake)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/installed)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_ENABLE_HARDENED "Enable hardened compiler options" OFF)
option(BUILD_TESTING "Enable building ${PROJECT_NAME} test applications." ON)

include(CTest)
include(GoogleTest)

add_subdirectory(app)
add_subdirectory(src)

# set(PROJECT_CONFIG_VERSION ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_ROOT_NAME}-config-version.cmake)
# write_basic_package_version_file(${PROJECT_CONFIG_VERSION} VERSION 0.0.0 COMPATIBILITY SameMajorVersion)

# install(EXPORT ${PROJECT_ROOT_NAME}-config DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})
# install(DIRECTORY include/${PROJECT_ROOT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
# install(FILES ${PROJECT_CONFIG_VERSION} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR})

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    find_program(CLANG_TIDY clang-tidy)

    if(CLANG_TIDY)
        include(ProcessorCount)
        ProcessorCount(NPROC)
        add_custom_target(clang-tidy
            COMMAND run-clang-tidy -p ${CMAKE_BINARY_DIR} -j ${NPROC} -source-filter="^${CMAKE_SOURCE_DIR}/\(app|src\)/.*"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Running clang-tidy on all files"
        )
    else()
        message(WARNING "clang-tidy executable not found - clang-tidy target will be skipped.")
    endif()

    find_program(LLVM_PROFDATA llvm-profdata)
    find_program(LLVM_COV llvm-cov)

    if(LLVM_PROFDATA AND LLVM_COV)
        get_property(PROJECT_LIB_TARGETS GLOBAL PROPERTY PROJECT_LIB_TARGETS)

        foreach(lib ${PROJECT_LIB_TARGETS})
            list(APPEND LLVM_COV_OBJECTS -object $<TARGET_FILE:${lib}>)
        endforeach()

        message(STATUS "Targets: ${LLVM_COV_OBJECTS}")

        add_custom_target(llvm-coverage
            # Merge raw profiles.
            COMMAND ${LLVM_PROFDATA} merge -sparse
                    ${CMAKE_BINARY_DIR}/coverage/*.profraw
                    -o ${CMAKE_BINARY_DIR}/coverage/merged.profdata

            COMMAND ${LLVM_COV} report
                -instr-profile=${CMAKE_BINARY_DIR}/coverage/merged.profdata
                ${LLVM_COV_OBJECTS}

            COMMAND ${LLVM_COV} show
                -format=html
                -output-dir=${CMAKE_BINARY_DIR}/coverage/html
                -instr-profile=${CMAKE_BINARY_DIR}/coverage/merged.profdata
                ${LLVM_COV_OBJECTS}
        )
    else()
        message(WARNING "llvm-profdata and llvm-cov executable(s) not found - llvm-coverage target will be skipped.")
    endif()
endif()

find_program(CLANG_FORMAT_EXECUTABLE clang-format QUIET)
if(CLANG_FORMAT_EXECUTABLE)
    file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/app/*
        ${CMAKE_SOURCE_DIR}/include/*
        ${CMAKE_SOURCE_DIR}/src/*
    )

    list(FILTER SOURCE_FILES INCLUDE REGEX ".*\\.(cpp|h|ixx)$")

    add_custom_target(clang-format-check
        COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run -Werror ${SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Checking format of source files..."
        VERBATIM
    )

    add_custom_target(clang-format
        COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${SOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Formating source files..."
        VERBATIM
    )
else()
    message(WARNING "Clang-format executable not found - clang-format target will be skipped.")
endif()

find_program(DOXYGEN_EXECUTABLE doxygen QUIET)
if(DOXYGEN_EXECUTABLE)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/doxygen/.doxyfile)
    set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/.doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating documentation with Doxygen..."
            VERBATIM
    )
    add_custom_command(
            TARGET doxygen
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Documentation generated at: ${CMAKE_BINARY_DIR}/docs/html/index.html"
    )
else()
    message(WARNING "Doxygen executable not found - doxygen target will be skipped.")
endif()
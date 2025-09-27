project(druid-graphics)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    shaders/default.vert
    shaders/default.frag
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Color.ixx
        Event.ixx
        Pipeline.ixx
        Renderer.ixx
        Vertex.ixx
        Window.ixx
)

find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    druid-core
    SDL3::SDL3
)

# Find glslangValidator, which is part of the Vulkan SDK
find_program(GLSLANG_VALIDATOR glslangValidator REQUIRED)
find_program(SPIRV_CROSS spirv-cross REQUIRED)

# Set shader source and output directories
set(SHADER_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADER_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)

# Create a list of shader files to compile
file(GLOB_RECURSE GLSL_SHADERS ${SHADER_SRC_DIR}/*.vert ${SHADER_SRC_DIR}/*.frag ${SHADER_SRC_DIR}/*.comp)

# Ensure the binary directory exists
file(MAKE_DIRECTORY ${SHADER_BIN_DIR})

# Function to compile shaders
foreach(SHADER ${GLSL_SHADERS})
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    set(SPIRV_BINARY ${SHADER_BIN_DIR}/${SHADER_NAME}.spv)
    
    add_custom_command(
        OUTPUT ${SPIRV_BINARY}
        COMMAND ${GLSLANG_VALIDATOR} -V ${SHADER} -o ${SPIRV_BINARY}
        DEPENDS ${SHADER}
        COMMENT "Compiling GLSL shader ${SHADER} to SPIR-V"
        VERBATIM
    )

    if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(MSL_FILE ${SHADER_BIN_DIR}/${SHADER_NAME}.msl)

        add_custom_command(
            OUTPUT ${MSL_FILE}
            COMMAND ${SPIRV_CROSS} ${SPIRV_BINARY} --msl --output ${MSL_FILE}
            DEPENDS ${SPIRV_BINARY}
            COMMENT "Compiling SPIR-V shader ${SPIRV_BINARY} to MSL"
            VERBATIM
        )

        list(APPEND SPIRV_BINARIES ${MSL_FILE})
    endif()

    list(APPEND SPIRV_BINARIES ${SPIRV_BINARY})
endforeach()

# Custom target to compile all shaders
add_custom_target(CompileShaders ALL DEPENDS ${SPIRV_BINARIES})

target_compile_definitions(${PROJECT_NAME} PUBLIC -DSHADER_DIR="${SHADER_BIN_DIR}")

project(druid-graphics)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Color.ixx
        Node.ixx
        NodeRectangle.ixx
        Renderer.ixx
        Window.ixx

        renderer/Raylib.ixx
)

find_package(glm CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    druid-core
    glm::glm
    raylib
    glfw
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        winmm
    )
endif()

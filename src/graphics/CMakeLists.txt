project(druid-graphics)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Color.cpp
    Window.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES
        ${CMAKE_SOURCE_DIR}/include/druid/graphics/Color.h
        ${CMAKE_SOURCE_DIR}/include/druid/graphics/NodeRectangle.h
        ${CMAKE_SOURCE_DIR}/include/druid/graphics/NodeText.h
        ${CMAKE_SOURCE_DIR}/include/druid/graphics/Renderer.h
        ${CMAKE_SOURCE_DIR}/include/druid/graphics/Window.h
        ${CMAKE_SOURCE_DIR}/include/druid/graphics/renderer/Raylib.h
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Node.ixx
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

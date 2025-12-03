project(druid-raylib)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES
        ${CMAKE_SOURCE_DIR}/include/druid/raylib/Drawable.h
        ${CMAKE_SOURCE_DIR}/include/druid/raylib/Module.h
        ${CMAKE_SOURCE_DIR}/include/druid/raylib/Window.h
)

target_sources(${PROJECT_NAME} PUBLIC
    Drawable.cpp
    Module.cpp
)

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    druid-core
    raylib
    glfw
)
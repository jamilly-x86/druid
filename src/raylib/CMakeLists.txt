project(druid-raylib)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Drawable.ixx
        Module.ixx
        Window.ixx
)

find_package(flecs CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
    glfw
    raylib
)
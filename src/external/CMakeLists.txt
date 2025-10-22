project(druid-external)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Flecs.ixx
)

find_package(flecs CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
)
project(druid-raylib)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET CXX_MODULES FILES
        Window.ixx
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        druid-core
        druid-graphics
        raylib
)

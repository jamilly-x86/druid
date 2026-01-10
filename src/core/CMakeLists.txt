project(druid-core)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Components.ixx
        Concepts.ixx
        Engine.ixx
        EnumMask.ixx
        Event.ixx
        File.ixx
        NotNull.ixx
        Object.ixx
        Signal.ixx
)

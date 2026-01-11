project(briarthorn)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

# target_sources(${PROJECT_NAME} PUBLIC
#     FILE_SET cxx_modules TYPE CXX_MODULES FILES
# )

# find_package(flecs CONFIG REQUIRED)
# find_package(magic_enum CONFIG REQUIRED)
# find_package(Microsoft.GSL CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    # druid-raylib
    # druid-anchor
    # $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
    # magic_enum::magic_enum
    # Microsoft.GSL::GSL
)

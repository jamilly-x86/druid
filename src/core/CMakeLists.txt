project(druid-core)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES
        ${CMAKE_SOURCE_DIR}/include/druid/core/Concepts.h
        ${CMAKE_SOURCE_DIR}/include/druid/core/Engine.h
        ${CMAKE_SOURCE_DIR}/include/druid/core/Event.h
        ${CMAKE_SOURCE_DIR}/include/druid/core/File.h
)

target_sources(${PROJECT_NAME} PRIVATE
    Engine.cpp
    File.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Components.ixx
        EnumMask.ixx
        Object.ixx
        Signal.ixx
)

find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
    Microsoft.GSL::GSL
)
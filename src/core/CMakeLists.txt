project(druid-core)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Color.cpp
    Engine.cpp
    File.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES
        ${CMAKE_SOURCE_DIR}/include/druid/core/Concepts.h
        ${CMAKE_SOURCE_DIR}/include/druid/core/Engine.h
        ${CMAKE_SOURCE_DIR}/include/druid/core/EnumMask.h
        ${CMAKE_SOURCE_DIR}/include/druid/core/Event.h
        ${CMAKE_SOURCE_DIR}/include/druid/core/File.h
        ${CMAKE_SOURCE_DIR}/include/druid/core/Object.h
        ${CMAKE_SOURCE_DIR}/include/druid/core/Signal.h
)

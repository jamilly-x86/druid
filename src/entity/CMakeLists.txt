project(druid-entity)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Entity.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES
        ${CMAKE_SOURCE_DIR}/include/druid/entity/Entity.h
        ${CMAKE_SOURCE_DIR}/include/druid/entity/World.h
)

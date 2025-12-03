project(druid-anchor)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES
        ${CMAKE_SOURCE_DIR}/include/druid/anchor/Components.h
        ${CMAKE_SOURCE_DIR}/include/druid/anchor/Module.h
)

target_sources(${PROJECT_NAME} PUBLIC
    Components.cpp
    Module.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-core
)
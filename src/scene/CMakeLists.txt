project(druid-scene)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Node.cpp
    Tree.cpp
)

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS ${CMAKE_SOURCE_DIR}/include
    FILES
        ${CMAKE_SOURCE_DIR}/include/druid/scene/Material.h
        ${CMAKE_SOURCE_DIR}/include/druid/scene/Mesh.h
        ${CMAKE_SOURCE_DIR}/include/druid/scene/Node.h
        ${CMAKE_SOURCE_DIR}/include/druid/scene/Transform.h
        ${CMAKE_SOURCE_DIR}/include/druid/scene/Tree.h
)

find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(flecs CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
    druid-core
    Microsoft.GSL::GSL
    glm::glm
    $<IF:$<TARGET_EXISTS:flecs::flecs>,flecs::flecs,flecs::flecs_static>
)

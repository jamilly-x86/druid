project(druid-entity-test)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Entity.test.cpp
    World.test.cpp
)

enable_testing()
    
find_package(GTest CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-entity
    GTest::gtest
    GTest::gtest_main
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})

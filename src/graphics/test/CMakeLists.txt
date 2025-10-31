project(druid-graphics-test)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Node.test.cpp
    main.cpp
)

enable_testing()
    
find_package(GTest CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-graphics
    GTest::gtest
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})

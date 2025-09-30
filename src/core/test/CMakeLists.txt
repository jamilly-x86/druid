project(druid-core-test)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    Object.test.cpp
    EnumMask.test.cpp
)

enable_testing()
    
find_package(GTest CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    druid-core
    GTest::gtest
    GTest::gtest_main
)

add_test(druid-test ${PROJECT_NAME})

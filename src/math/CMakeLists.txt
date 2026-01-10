project(druid-math)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Vec2.ixx
        Vec3.ixx
        Mat4.ixx
)

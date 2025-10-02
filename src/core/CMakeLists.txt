project(druid-core)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Concepts.ixx
        EnumMask.ixx
        File.ixx
        Object.ixx
)

find_package(PalSigslot CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Pal::Sigslot
)

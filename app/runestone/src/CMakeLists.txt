project(runestone-modules)

project_add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        ChessBoard.ixx
        ChessPiece.ixx
        Position.ixx
)

find_package(magic_enum CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    magic_enum::magic_enum
)
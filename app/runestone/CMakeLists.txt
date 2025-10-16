project(runestone)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
        ChessBoard.h
        main.cpp
        Types.h
)
project(runestone)

project_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

target_sources(${PROJECT_NAME} PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        ChessBoard.ixx
)
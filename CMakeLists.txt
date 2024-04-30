cmake_minimum_required(VERSION 3.25.1)
project(lane-detector)

set(CMAKE_CXX_STANDARD_REQUIRED 20)

find_package (OpenCV REQUIRED)

add_executable(${PROJECT_NAME})

target_include_directories (${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${OpenCV_INCLUDE_DIRS}
)

target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
)

target_link_libraries (${PROJECT_NAME} PRIVATE
    ${OpenCV_LIBS}
)
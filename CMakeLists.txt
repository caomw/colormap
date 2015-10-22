cmake_minimum_required(VERSION 2.8)
project(colormap)


# Set C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# add source files
file(GLOB SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/*.cc"
    "${PROJECT_SOURCE_DIR}/libcolormap/*.h"
)

# main executable
add_executable(colormap ${SOURCE_FILES})


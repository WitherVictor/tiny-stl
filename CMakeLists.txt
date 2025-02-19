cmake_minimum_required(VERSION 3.21)
project(tiny-stl VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -pedantic
    -std=c++26
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
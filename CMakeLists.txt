cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(OpenHashTable VERSION 1.0)

add_library(includes INTERFACE)
target_include_directories(includes INTERFACE include)

add_executable(OpenHashTable main.cpp)
target_link_libraries(OpenHashTable PUBLIC includes)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(OpenHashTable VERSION 1.0)

add_library(includes src/hash_functions.cpp)
target_include_directories(includes PUBLIC include)

add_executable(OpenHashTable main.cpp)
target_link_libraries(OpenHashTable PUBLIC includes)

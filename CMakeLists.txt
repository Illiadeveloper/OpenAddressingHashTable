cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(OpenHashTable VERSION 1.0)

# --- Hash Functions ---
add_library(hashFunc hash_functions/hash_functions.cpp)
target_include_directories(hashFunc PUBLIC hash_functions)

# --- Hash Table---
add_library(hashTable INTERFACE)
target_include_directories(hashTable INTERFACE hash_table)

add_executable(OpenHashTable main.cpp)

target_link_libraries(OpenHashTable PUBLIC hashFunc hashTable)

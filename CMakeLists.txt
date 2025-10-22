cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(OpenHashTable VERSION 1.3)

add_library(includes INTERFACE)
target_include_directories(includes INTERFACE include)

# ==== Tests ====
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
  message(STATUS "Building with hash table tests")
  enable_testing()
  add_subdirectory(tests)
endif()

# ==== STATISTICS ====
option(HASH_TABLE_STATISTIC "Enable hash statistic test" ON)
if(HASH_TABLE_STATISTIC)
  message(STATUS "Building with hash table statistics")
  add_definitions(-DHASH_TABLE_STATISTIC)
  add_executable(OpenHashTable test_statistics.cpp)
endif()

target_link_libraries(OpenHashTable PUBLIC includes)

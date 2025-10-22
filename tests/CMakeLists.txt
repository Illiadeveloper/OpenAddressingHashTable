cmake_policy(SET CMP0135 NEW)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(
  HashTableTests
  test_open_addressing_hash_table.cpp
)
target_link_libraries(
  HashTableTests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(HashTableTests)

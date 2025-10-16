#include "open_addressing_hash_table.h"
#include <iostream>
#include <utility>
// cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cmake --build build
// && ./build/OpenHashTable

int main(int argc, char *argv[]) {
  OpenAddressingHashTable<int, char> table1 = {
      {5, 'c'}, {-11, 'a'}, {6, 'b'}, {40, 'h'}, {80, 'm'}};
  OpenAddressingHashTable<int, char> table2 = {{100, 'a'}, {200, 'b'}};
  // OpenAddressingHashTable<int, char> table2;
  table2 = table1;
  std::cout << "TABLE 1:";
  for (auto &x : table1) {
    std::cout << x.value << " ";
  }
  std::cout << std::endl;

  std::cout <<"IS EQUAL: " << (table1 == table2) << std::endl;

  std::cout << "TABLE 2:";
  for (auto &x : table2) {
    std::cout << x.value << " ";
  }
  std::cout << std::endl;

  return 0;
}

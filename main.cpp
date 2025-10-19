#include "hash_functions.h"
#include "open_addressing_hash_table.h"
#include <iostream>
#include <utility>
// cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cmake --build build
// && ./build/OpenHashTable

int main(int argc, char *argv[]) {
  OpenAddressingHashTable<int, int, Hash<int>, QuadraticHashing<int>> table;
  for (int i = 0; i < 150; i += 2) {
    table.insert(i, i * 100);
  }

  for (auto &x : table) {
    std::cout << x.value << " ";
  }
  std::cout << std::endl;

  return 0;
}

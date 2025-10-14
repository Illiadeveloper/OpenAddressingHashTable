#include "open_addressing_hash_table.h"
#include <iostream>
#include <unordered_map>
//cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cmake --build build && ./build/OpenHashTable

int main (int argc, char *argv[]) {
  OpenAddressingHashTable<int, char> table;
  table.insert(5, 'c');
  table.insert(-11, 'a');
  table.insert(5, 'b');
  table.insert(40, 'h');
  table.insert(80, 'm');

  for(auto& x : table) {
    std::cout << x.value << " ";
  }
  std::cout << std::endl;
  for(auto& x : table.get_container()) {
    std::cout << x.value << " ";
  }
  std::cout << std::endl;
  return 0;
}

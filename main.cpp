#include "open_addressing_hash_table.h"
#include <iostream>
//cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cmake --build build && ./build/OpenHashTable

int main (int argc, char *argv[]) {
  OpenAddressingHashTable<int, char> table;
  table[5] = 'c';
  table[-11] = 'a';

  table.insert(6, 'b');
  table.insert(40, 'h');
  table.insert(80, 'm');

  for(auto& x : table) {
    std::cout << x.value << " ";
  }
  std::cout << std::endl;

  std::cout << "SIZE: "<< table.size() << std::endl;
  std::cout << "IS EMPTY: "<< table.empty() << std::endl;
  table.clear();
  for(auto& x : table) {
    std::cout << x.value << " ";
  }
  std::cout << std::endl;

  std::cout << "IS EMPTY: "<< table.empty() << std::endl;

  return 0;
}

#include "hash_functions/hash_functions.h"

int main (int argc, char *argv[]) {
  std::cout << HashFunctions::hash(12.121f) << std::endl;
  std::cout << HashFunctions::hash(48) << std::endl;
  std::cout << HashFunctions::hash('0') << std::endl;
  return 0;
}

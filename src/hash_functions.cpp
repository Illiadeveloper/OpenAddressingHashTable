#include "hash_functions.h"

size_t HashFunctions::hash(int key) { return (key * 2654435761u); }
size_t HashFunctions::hash(char key) {
  int hash = static_cast<int>(key << 8);
  hash *= 0xF5;
  return HashFunctions::hash(hash);
}

size_t HashFunctions::hash(float key) {
  if (key == 0.0f)
    key = 0.0f;
  uint32_t bits;
  std::memcpy(&bits, &key, sizeof(bits));

  bits ^= bits >> 17;
  bits *= 0xEF59A5B3;
  bits ^= bits >> 11;
  bits *= 0xAC4C1B51;
  bits ^= bits >> 15;
  bits *= 0x31848BAB;
  bits ^= bits >> 14;

  return static_cast<size_t>(bits);
}

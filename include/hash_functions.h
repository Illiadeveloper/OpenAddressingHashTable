#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>

template <typename T> struct Hash {
  size_t operator()(const T &key) const;
};

template <> struct Hash<int> {
  size_t operator()(int key) const { return key * 2654435761u; }
};

template <> struct Hash<char> {
  size_t operator()(int key) const {
    int hash = static_cast<int>(key << 8);
    hash *= 0xF5;
    return hash * 2654435761u;
  }
};

template <> struct Hash<float> {
  size_t operator()(int key) const {
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
};

#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

template <typename T> struct Hash {
  size_t operator()(const T &key) const;
};

template <> struct Hash<int> {
  size_t operator()(int key) const { return key * 2654435761u; }
};

template <> struct Hash<char> {
  size_t operator()(char key) const {
    int hash = static_cast<int>(key << 8);
    hash *= 0xF5;
    return hash * 2654435761u;
  }
};

template <> struct Hash<float> {
  size_t operator()(float key) const {
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
template <> struct Hash<std::string> {
  size_t operator()(const std::string &key) const {
    const uint64_t seed = 0xc70f6907UL;
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^ (key.size() * m);
    const uint64_t *data = (const uint64_t *)key.data();
    const uint64_t *end = data + (key.size() / 8);

    while (data != end) {
      uint64_t k = *data++;
      k *= m;
      k ^= k >> r;
      k *= m;
      h ^= k;
      h *= m;
    }

    const unsigned char *data2 = (const unsigned char *)data;
    switch (key.size() & 7) {
    case 7:
      h ^= uint64_t(data2[6]) << 48;
    case 6:
      h ^= uint64_t(data2[5]) << 40;
    case 5:
      h ^= uint64_t(data2[4]) << 32;
    case 4:
      h ^= uint64_t(data2[3]) << 24;
    case 3:
      h ^= uint64_t(data2[2]) << 16;
    case 2:
      h ^= uint64_t(data2[1]) << 8;
    case 1:
      h ^= uint64_t(data2[0]);
      h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return static_cast<size_t>(h);
  }
};

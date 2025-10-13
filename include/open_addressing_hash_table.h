#pragma once
#include <vector>
#include "hash_functions.h"

template <typename K, typename V> struct Entry {
  K key;
  V value;
  bool occupied;
  bool deleted;
};

template <typename K, typename V> 
class OpenAddressingHashTable {
public:
  OpenAddressingHashTable();
  void insert(K key, V value);
  void erase(K key);
  V *find(K key);

  void rehash(size_t new_capacity);

private:
  std::vector<Entry<K, V>> data;
  int size;
};

template <typename K, typename V>
OpenAddressingHashTable<K, V>::OpenAddressingHashTable() : size(0) {
  data.resize(4);
}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::insert(K key, V value) {
  if ((float)size / data.size() > 0.7f) {
    rehash(data.size() * 2);
  }
}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::erase(K key) {}

template <typename K, typename V>
V *OpenAddressingHashTable<K, V>::find(K key) {}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::rehash(size_t new_capacity) {
  std::vector<Entry<K, V>> new_data(new_capacity);
  for (int i = 0; i < data.size(); ++i) {
    // if (data[i].occupied && !data[i].deleted) {
    //   size_t new_index
    // }
  }
}

#pragma once
#include <vector>

template <typename K, typename V>
struct Entry {
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
  V* find(K key);
  
  void resize(size_t n);
  void rehash();
private:
  std::vector<Entry<K, V>> data;
  int size;
};

template <typename K, typename V>
OpenAddressingHashTable<K, V>::OpenAddressingHashTable()
    : size(0) {}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::insert(K key, V value) {}


template <typename K, typename V>
void OpenAddressingHashTable<K, V>::erase(K key) {}

template <typename K, typename V>
V* OpenAddressingHashTable<K, V>::find(K key) {}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::resize(size_t n) {}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::rehash() {}

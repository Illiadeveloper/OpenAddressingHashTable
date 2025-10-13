#pragma once
#include "hash_functions.h"
#include <vector>

enum class EntryState { EMPTY, OCCUPIED, DELETED };

template <typename K, typename V> //
struct Entry {
  K key;
  V value;
  EntryState state = EntryState::EMPTY;
};

template <typename K, typename V> //
class OpenAddressingHashTable {
public:
  OpenAddressingHashTable();
  void insert(K key, V value);
  void erase(K key);
  V *find(K key);

  void rehash(size_t new_capacity);
  std::vector<Entry<K, V>> get_container() const;

private:
  std::vector<Entry<K, V>> data;
  Hash<K> hasher;
  size_t num_elements = 0;
};

template <typename K, typename V>
OpenAddressingHashTable<K, V>::OpenAddressingHashTable()
    : num_elements(0), data(4) {}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::insert(K key, V value) {
  if ((float)num_elements / data.size() > 0.7f) {
    rehash(data.size() * 2);
  }
  int index = hasher(key) & (data.size() - 1);

  while (data[index].state == EntryState::OCCUPIED) {
    index = (index + 1) & (data.size() - 1);
  }

  data[index].value = value;
  data[index].key = key;
  data[index].state = EntryState::OCCUPIED;
  num_elements++;
  return;
}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::erase(K key) {}

template <typename K, typename V>
V *OpenAddressingHashTable<K, V>::find(K key) {}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::rehash(size_t new_capacity) {
  std::vector<Entry<K, V>> new_data(new_capacity);
  for (size_t i = 0; i < data.size(); ++i) {
    if (data[i].state == EntryState::OCCUPIED) {
      size_t new_index = hasher(data[i].key) & (new_data.size() - 1);

      while (new_data[new_index].state == EntryState::OCCUPIED) {
        new_index = (new_index + 1) & (new_data.size() - 1);
      }

      new_data[new_index].key = data[i].key;
      new_data[new_index].value = data[i].value;
      new_data[new_index].state = EntryState::OCCUPIED;
    }
  }
  data = std::move(new_data);
}

template <typename K, typename V>
std::vector<Entry<K, V>> OpenAddressingHashTable<K, V>::get_container() const {
  return data;
}

#pragma once
#include "hash_functions.h"
#include <cstddef>
#include <iterator>
#include <locale>
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
  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Entry<K, V>;
    using pointer = value_type *;
    using reference = value_type &;

    iterator(pointer ptr, pointer end_ptr) : current(ptr), end(end_ptr) {}

    iterator operator++() {
      ++current;
      skip_empty();
      return *this;
    };

    iterator operator++(int) {
      pointer temp = *this;
      ++current;
      skip_empty();
      return temp;
    };

    bool operator==(const iterator &other) { return current == other.current; }
    bool operator!=(const iterator &other) { return !(*this == other); }

    reference operator*() const { return *current; }
    pointer operator->() const { return current; };

  private:
    pointer current;
    pointer end;

    void skip_empty() {
      while (current != end && current->state != EntryState::OCCUPIED) {
        ++current;
      }
    }
  };

  class const_iterator {};

  // Usings for STD cointainers
  using key_type = K;
  using mapped_type = V;
  using value_type = Entry<K, V>;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;

  using iterator = iterator;

  OpenAddressingHashTable() : num_elements(0), data(4) {}

  iterator begin() { return iterator(&data.front(), &data.back() + 1); };
  iterator end() { return iterator(&data.back() + 1, &data.back() + 1); };

  void insert(key_type key, mapped_type value);
  void erase(key_type key);
  iterator find(key_type key);

  void rehash(size_type new_capacity);

  std::vector<Entry<K, V>> get_container() const { return data; }

private:
  std::vector<Entry<K, V>> data;
  Hash<K> hasher;
  size_t num_elements = 0;
};

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::insert(key_type key, mapped_type value) {
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
void OpenAddressingHashTable<K, V>::erase(key_type key) {}

template <typename K, typename V>
typename OpenAddressingHashTable<K, V>::iterator
OpenAddressingHashTable<K, V>::find(key_type key) {}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::rehash(size_type new_capacity) {
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

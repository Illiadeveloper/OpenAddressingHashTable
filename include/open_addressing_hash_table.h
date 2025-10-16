#pragma once
#include "hash_functions.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

enum class EntryState { EMPTY, OCCUPIED, DELETED };

constexpr float LOAD_FACTOR = 0.7f;
constexpr float DELETE_FACTOR = 0.3f;

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

    iterator(pointer ptr, pointer end_ptr) : current(ptr), end(end_ptr) {
      skip_empty();
    }

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

  OpenAddressingHashTable() : num_elements(0), data(4), num_deleted(0) {}
  OpenAddressingHashTable(std::initializer_list<std::pair<const K, V>> init)
      : num_elements(0), num_deleted(0), data(4) {
    for (auto &p : init)
      insert(p.first, p.second);
  }
  OpenAddressingHashTable(OpenAddressingHashTable &&other)
      : num_elements(other.num_elements), data(std::move(other.data)),
        num_deleted(other.num_deleted), hasher(std::move(other.hasher)) {
    other.num_deleted = 0;
    other.num_elements = 0;
  }
  OpenAddressingHashTable(const OpenAddressingHashTable &other)
      : num_elements(other.num_elements), data(other.data),
        num_deleted(other.num_deleted), hasher(other.hasher) {}

  iterator begin() noexcept {
    return iterator(data.data(), data.data() + data.size());
  };
  iterator end() noexcept {
    return iterator(data.data() + data.size(), data.data() + data.size());
  };

  void insert(key_type key, mapped_type value);
  size_type erase(key_type key);
  iterator find(key_type key);

  void operator=(const OpenAddressingHashTable &other);
  void operator=(OpenAddressingHashTable &&other);

  bool operator==(OpenAddressingHashTable &other);
  bool operator!=(OpenAddressingHashTable &other);

  mapped_type &operator[](key_type key);
  mapped_type &at(key_type key);

  void rehash(size_type new_capacity);

  std::vector<Entry<K, V>> get_container() const { return data; }

  size_type size() const noexcept { return num_elements; }
  bool empty() const noexcept { return num_elements == 0 ? 1 : 0; }

  void clear() noexcept;
  bool contains(const key_type &key) const;

  void swap(OpenAddressingHashTable<K, V> &other);

private:
  std::vector<Entry<K, V>> data;
  Hash<K> hasher;
  size_t num_elements;
  size_t num_deleted;
};

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::insert(key_type key, mapped_type value) {
  if ((float)num_elements / data.size() > LOAD_FACTOR) {
    rehash(data.size() * 2);
  }
  int index = hasher(key) & (data.size() - 1);

  while (data[index].state == EntryState::OCCUPIED) {
    index = (index + 1) & (data.size() - 1);
  }

  if (data[index].state == EntryState::DELETED) {
    num_deleted--;
  }

  data[index].value = value;
  data[index].key = key;
  data[index].state = EntryState::OCCUPIED;
  num_elements++;
  return;
}
/// ================== OPERATORS ================
template <typename K, typename V>
typename OpenAddressingHashTable<K, V>::mapped_type &
OpenAddressingHashTable<K, V>::operator[](key_type key) {
  size_t index = hasher(key) & (data.size() - 1);
  while (data[index].state != EntryState::EMPTY) {
    if (data[index].state == EntryState::OCCUPIED && data[index].key == key) {
      return data[index].value;
    }
    index = (index + 1) & (data.size() - 1);
  }
  if (static_cast<float>(num_elements) / data.size() > LOAD_FACTOR) {
    rehash(data.size() * 2);
    index = hasher(key) & (data.size() - 1);
  }
  data[index].value = mapped_type{};
  data[index].key = key;
  data[index].state = EntryState::OCCUPIED;
  num_elements++;
  return data[index].value;
}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::operator=(
    const OpenAddressingHashTable &other) {
  data = other.data;
  num_deleted = other.num_deleted;
  num_elements = other.num_elements;
  hasher = other.hasher;
}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::operator=(OpenAddressingHashTable &&other) {
  data = std::move(other.data);
  hasher = std::move(other.hasher);

  num_deleted = other.num_deleted;
  num_elements = other.num_elements;

  other.num_elements = 0;
  other.num_deleted = 0;
}

template <typename K, typename V>
bool OpenAddressingHashTable<K, V>::operator==(OpenAddressingHashTable &other) {

  if (other.num_elements != num_elements)
    return false;

  for (auto &entry : data) {
    if (entry.state != EntryState::OCCUPIED)
      continue;
    iterator it = other.find(entry.key);
    if (it == other.end())
      return false;

    if (it->value != entry.value)
      return false;
  }
  return true;
}

template <typename K, typename V>
bool OpenAddressingHashTable<K, V>::operator!=(OpenAddressingHashTable &other) {
  return !(*this == other);
}

template <typename K, typename V>
typename OpenAddressingHashTable<K, V>::mapped_type &
OpenAddressingHashTable<K, V>::at(key_type key) {
  size_t index = hasher(key) & (data.size() - 1);
  while (data[index].state != EntryState::EMPTY) {
    if (data[index].state == EntryState::OCCUPIED && data[index].key == key) {
      return data[index].value;
    }
    index = (index + 1) & (data.size() - 1);
  }
  throw std::out_of_range("function at(): key was not found");
}

template <typename K, typename V>
typename OpenAddressingHashTable<K, V>::size_type
OpenAddressingHashTable<K, V>::erase(key_type key) {
  if (static_cast<float>(num_deleted) / data.size() > DELETE_FACTOR) {
    rehash(data.size());
    num_deleted = 0;
  }

  size_t index = hasher(key) & (data.size() - 1);
  while (data[index].state != EntryState::EMPTY) {
    if (data[index].state == EntryState::OCCUPIED && data[index].key == key) {
      data[index].state = EntryState::DELETED;
      --num_elements;
      num_deleted++;
      return 1;
    }
    index = (index + 1) & (data.size() - 1);
  }
  return 0;
}

template <typename K, typename V>
typename OpenAddressingHashTable<K, V>::iterator
OpenAddressingHashTable<K, V>::find(key_type key) {
  size_t index = hasher(key) & (data.size() - 1);
  while (data[index].state != EntryState::EMPTY) {
    if (data[index].state == EntryState::OCCUPIED && data[index].key == key) {
      return iterator(&*(data.begin() + index), &data.back() + 1);
    }
    index = (index + 1) & (data.size() - 1);
  }
  return end();
}

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

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::clear() noexcept {
  for (size_type i = 0; i < data.size(); ++i) {
    data[i].state = EntryState::EMPTY;
  }
  num_elements = 0;
}
template <typename K, typename V>
bool OpenAddressingHashTable<K, V>::contains(const key_type &key) const {
  size_type index = hasher(key) & (data.size() - 1);
  while (data[index].state != EntryState::EMPTY) {
    if (data[index].key == key && data[index].state == EntryState::OCCUPIED) {
      return true;
    }
    index = (index + 1) & (data.size() - 1);
  }
  return false;
}

template <typename K, typename V>
void OpenAddressingHashTable<K, V>::swap(OpenAddressingHashTable<K, V> &other) {
  std::swap(num_elements, other.num_elements);
  std::swap(hasher, other.hasher);
  std::swap(data, other.data);
}

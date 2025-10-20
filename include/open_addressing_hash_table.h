#pragma once
#include <algorithm>
#include <cstddef>
#include <functional>
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

template <typename K> struct LinearHashing {
  size_t operator()(size_t hash, size_t i, size_t table_size) const {
    return (hash + i) & (table_size - 1);
  }
};

template <typename K> struct QuadraticHashing {
  size_t operator()(size_t hash, size_t i, size_t table_size) const {
    return (hash + (i + i * i) / 2) & (table_size - 1);
  }
};

template <typename K> struct DoubleHashing {
  size_t operator()(size_t hash1, size_t i, size_t table_size) const {
    size_t hash2 = ((hash1 >> 16) | 1);
    return (hash1 + i * hash2) & (table_size - 1);
  }
};

template <typename K, typename V, typename HashFunction = std::hash<K>,
          typename ProbingPolicy = LinearHashing<K>> //
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

    bool operator==(const iterator &other) const { return current == other.current; }
    bool operator!=(const iterator &other) const { return !(*this == other); }

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

  void swap(OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy> &other);

private:
  std::vector<Entry<K, V>> data;
  HashFunction hasher;
  ProbingPolicy probe;
  size_t num_elements;
  size_t num_deleted;
};

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
void OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::insert(
    key_type key, mapped_type value) {
  if ((float)num_elements / data.size() > LOAD_FACTOR) {
    rehash(data.size() * 2);
  }
  size_t hash = hasher(key);
  size_t i = 0;
  size_t index = probe(hash, i, data.size());

  while (data[index].state == EntryState::OCCUPIED) {
    index = probe(hash, ++i, data.size());
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
template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
typename OpenAddressingHashTable<K, V, HashFunction,
                                 ProbingPolicy>::mapped_type &
OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::operator[](
    key_type key) {
  size_t hash = hasher(key);
  size_t i = 0;
  size_t index = probe(hash, i, data.size());

  while (data[index].state != EntryState::EMPTY) {
    if (data[index].state == EntryState::OCCUPIED && data[index].key == key) {
      return data[index].value;
    }
    index = probe(hash, ++i, data.size());
  }
  if (static_cast<float>(num_elements) / data.size() > LOAD_FACTOR) {
    rehash(data.size() * 2);
    i = 0;
    index = probe(hash, ++i, data.size());
  }
  data[index].value = mapped_type{};
  data[index].key = key;
  data[index].state = EntryState::OCCUPIED;
  num_elements++;
  return data[index].value;
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
void OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::operator=(
    const OpenAddressingHashTable &other) {
  data = other.data;
  num_deleted = other.num_deleted;
  num_elements = other.num_elements;
  hasher = other.hasher;
  probe = other.probe;
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
void OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::operator=(
    OpenAddressingHashTable &&other) {
  data = std::move(other.data);
  hasher = std::move(other.hasher);
  probe = std::move(other.probe);

  num_deleted = other.num_deleted;
  num_elements = other.num_elements;

  other.num_elements = 0;
  other.num_deleted = 0;
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
bool OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::operator==(
    OpenAddressingHashTable &other) {
  if (other.num_elements != num_elements)
    return false;

  for (auto &entry : data) {
    if (entry.state != EntryState::OCCUPIED)
      continue;

    iterator it = other.find(entry.key);
    if (it == other.end() || it->value != entry.value)
      return false;
  }
  return true;
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
bool OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::operator!=(
    OpenAddressingHashTable &other) {
  return !(*this == other);
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
typename OpenAddressingHashTable<K, V, HashFunction,
                                 ProbingPolicy>::mapped_type &
OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::at(key_type key) {
  size_t hash = hasher(key);
  size_t i = 0;
  size_t index = probe(hash, i, data.size());

  while (data[index].state != EntryState::EMPTY) {
    if (data[index].state == EntryState::OCCUPIED && data[index].key == key)
      return data[index].value;

    ++i;
    index = probe(hash, i, data.size());
  }
  throw std::out_of_range("function at(): key was not found");
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
typename OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::size_type
OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::erase(
    key_type key) {
  if (static_cast<float>(num_deleted) / data.size() > DELETE_FACTOR) {
    rehash(data.size());
    num_deleted = 0;
  }

  size_t hash = hasher(key);
  size_t i = 0;
  size_t index = probe(hash, i, data.size());

  while (data[index].state != EntryState::EMPTY) {
    if (data[index].state == EntryState::OCCUPIED && data[index].key == key) {
      data[index].state = EntryState::DELETED;
      --num_elements;
      ++num_deleted;
      return 1;
    }
    ++i;
    index = probe(hash, i, data.size());
  }
  return 0;
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
typename OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::iterator
OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::find(key_type key) {
  size_t hash = hasher(key);
  size_t i = 0;
  size_t index = probe(hash, i, data.size());

  while (data[index].state != EntryState::EMPTY) {
    if (data[index].state == EntryState::OCCUPIED && data[index].key == key)
      return iterator(&*(data.begin() + index), &data.back() + 1);

    ++i;
    index = probe(hash, i, data.size());
  }
  return end();
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
void OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::rehash(
    size_type new_capacity) {
  std::vector<Entry<K, V>> new_data(new_capacity);

  for (auto &entry : data) {
    if (entry.state != EntryState::OCCUPIED)
      continue;

    size_t hash = hasher(entry.key);
    size_t i = 0;
    size_t new_index = probe(hash, i, new_data.size());

    while (new_data[new_index].state == EntryState::OCCUPIED) {
      ++i;
      new_index = probe(hash, i, new_data.size());
    }

    new_data[new_index] = entry;
  }

  data = std::move(new_data);
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
void OpenAddressingHashTable<K, V, HashFunction,
                             ProbingPolicy>::clear() noexcept {
  for (auto &entry : data)
    entry.state = EntryState::EMPTY;

  num_elements = 0;
  num_deleted = 0;
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
bool OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::contains(
    const key_type &key) const {
  size_t hash = hasher(key);
  size_t i = 0;
  size_t index = probe(hash, i, data.size());

  while (data[index].state != EntryState::EMPTY) {
    if (data[index].state == EntryState::OCCUPIED && data[index].key == key)
      return true;

    ++i;
    index = probe(hash, i, data.size());
  }
  return false;
}

template <typename K, typename V, typename HashFunction, typename ProbingPolicy>
void OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy>::swap(
    OpenAddressingHashTable<K, V, HashFunction, ProbingPolicy> &other) {
  std::swap(num_elements, other.num_elements);
  std::swap(num_deleted, other.num_deleted);
  std::swap(hasher, other.hasher);
  std::swap(probe, other.probe);
  std::swap(data, other.data);
}

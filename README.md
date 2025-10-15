# 🧠 Develop Plan - Open Addresing Hash Table

A C++ implementation of a **hash table** using *open addressing* with linear probing.  
The goal of the project is to explore and implement the internal mechanisms of hash tables — from iterators and probing strategies to rehashing and efficiency analysis.

---

## ✅ Done
- ✅ **Minimal Hash Table functions** (`insert`, `delete`, `find`)
- ✅ **Iterator**
- ✅ **Minimal Hash function**

---

## 🚧 In Progress / Planned

- ⬜ **operator[]** and **at()** — standard-style accessors  
- ⬜ **const_iterator** — read-only iteration support  
- ⬜ **Lazy rehash** — rehashing when too many tombstones accumulate  
- ⬜ **Move semantics** — improve rehash and copy efficiency  
- ⬜ **Utility methods:**  
  - `size()`, `empty()`, `clear()`, `contains()`, `count()`, `swap()`  
- ⬜ **Collision statistics** — track and print probing/collision data  
- ⬜ **Quadratic and double hashing** — additional probing strategies  
- ⬜ **Unit tests** — coverage for all table operations  
- ⬜ **Template extensions:** custom **allocator**, **hash function**, **key equality**

---

## 💡 Maybe Later (Experimental Ideas)

- ⬜ **Thread-safe operations** using `std::shared_mutex` and `std::lock_guard`  
- ⬜ **Robin Hood hashing** — equalizing probe distances  

---

## 🧩 Notes

- **Language:** C++20  
- **Build system:** CMake 
- **Structure:** Header-only library  
- **Current branch:** `develop`  
- **Goal:** Complete STL-like hash table API with open addressing  

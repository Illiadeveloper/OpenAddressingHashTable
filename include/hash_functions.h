#pragma once
#include <cstddef>
#include <iostream>
#include <cstdint>
#include <cstring>

class HashFunctions {
public:
    static size_t hash(int key);
    static size_t hash(char key);
    static size_t hash(float key);
};

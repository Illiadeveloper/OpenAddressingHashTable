#ifdef HASH_TABLE_STATISTIC
#include "hash_functions.h"
#include "open_addressing_hash_table.h"
#include <fstream>
#include <iostream>

class StatisticWriter {
public:
  StatisticWriter(const std::string &filename) : filename(filename) {
    std::ofstream file(filename, std::ios::trunc);
    file << "key_type,probing,inserted,insert_collisions,rehash_count,rehash_"
            "collisions\n";
  }

  void append(const std::string &keyType, const std::string &probing,
              int inserted, int insertCollisions, int rehashCount,
              int rehashCollisions) {
    std::ofstream file(filename, std::ios::app);
    file << keyType << "," << probing << "," << inserted << ","
         << insertCollisions << "," << rehashCount << "," << rehashCollisions
         << "\n";
  }

private:
  std::string filename;
};

void testStringLinear(StatisticWriter &stat) {
  OpenAddressingHashTable<std::string, int, Hash<std::string>,
                          LinearHashing<std::string>>
      table;
  for (int i = 0; i < 2000; ++i) {
    std::string key = "key_" + std::to_string(i);
    table.insert(key, i);

    if (i % 20 == 0) {
      stat.append("string", "linear", i, table.getInsertCollisions(),
                  table.getRehashCount(), table.getRehashCollisions());
    }
  }
}

void testStringDouble(StatisticWriter &stat) {
  OpenAddressingHashTable<std::string, int, Hash<std::string>,
                          DoubleHashing<std::string>>
      table;
  for (int i = 0; i < 2000; ++i) {
    std::string key = "key_" + std::to_string(i);
    table.insert(key, i);

    if (i % 20 == 0) {
      stat.append("string", "double", i, table.getInsertCollisions(),
                  table.getRehashCount(), table.getRehashCollisions());
    }
  }
}

void testStringQuadratic(StatisticWriter &stat) {
  OpenAddressingHashTable<std::string, int, Hash<std::string>,
                          QuadraticHashing<std::string>>
      table;
  for (int i = 0; i < 2000; ++i) {
    std::string key = "key_" + std::to_string(i);
    table.insert(key, i);

    if (i % 20 == 0) {
      stat.append("string", "quadratic", i, table.getInsertCollisions(),
                  table.getRehashCount(), table.getRehashCollisions());
    }
  }
}

void testIntDouble(StatisticWriter &stat) {
  OpenAddressingHashTable<int, int, Hash<int>, DoubleHashing<int>> table;
  for (int i = 0; i < 5000; i += 10) {
    table.insert(i, i * 10);

    if (i % 100 == 0) {
      stat.append("int", "double", i, table.getInsertCollisions(),
                  table.getRehashCount(), table.getRehashCollisions());
    }
  }
}

void testIntLinear(StatisticWriter &stat) {
  OpenAddressingHashTable<int, int, Hash<int>, LinearHashing<int>> table;
  for (int i = 0; i < 5000; i += 10) {
    table.insert(i, i * 10);

    if (i % 100 == 0) {
      stat.append("int", "linear", i, table.getInsertCollisions(),
                  table.getRehashCount(), table.getRehashCollisions());
    }
  }
}

void testIntQuadratic(StatisticWriter &stat) {
  OpenAddressingHashTable<int, int, Hash<int>, QuadraticHashing<int>>
      table;
  for (int i = 0; i < 5000; i += 10) {
    table.insert(i, i * 10);

    if (i % 100 == 0) {
      stat.append("int", "quadratic", i, table.getInsertCollisions(),
                  table.getRehashCount(), table.getRehashCollisions());
    }
  }
}

int main() {
  StatisticWriter stat("hash_stats.csv");

  // STRING TESTS
  testStringLinear(stat);
  testStringDouble(stat);
  testStringQuadratic(stat);

  // INT TESTS
  testIntDouble(stat);
  testIntLinear(stat);
  testIntQuadratic(stat);

  std::cout << "Statistics saved to hash_stats.csv\n";
  return 0;
}
#endif // HASH_TABLE_STATISTIC

#include "open_addressing_hash_table.h"
#include <gtest/gtest.h>

TEST(OpenAddressingHashTableTest, InsertAndFind) {
  OpenAddressingHashTable<int, std::string> table;

  table.insert(1, "One");
  table.insert(2, "Two");
  table.insert(3, "Three");

  auto it = table.find(2);
  ASSERT_NE(it, table.end());
  EXPECT_EQ(it->value, "Two");

  auto it2 = table.find(42);
  EXPECT_EQ(it2, table.end());
}

TEST(OpenAddressingHashTableTest, OperatorIndexing) {
  OpenAddressingHashTable<int, std::string> table;

  table[10] = "Ten";
  EXPECT_EQ(table[10], "Ten");

  EXPECT_EQ(table[99], "");
  EXPECT_TRUE(table.contains(99));
}

TEST(OpenAddressingHashTableTest, EraseTest) {
  OpenAddressingHashTable<int, std::string> table;

  table.insert(1, "One");
  table.insert(2, "Two");
  table.insert(3, "Three");

  EXPECT_EQ(table.erase(2), 1);
  EXPECT_FALSE(table.contains(2));

  EXPECT_EQ(table.erase(999), 0);
}

TEST(OpenAddressingHashTableTest, ClearAndEmpty) {
  OpenAddressingHashTable<int, std::string> table{{1, "a"}, {2, "b"}};

  EXPECT_FALSE(table.empty());
  table.clear();
  EXPECT_TRUE(table.empty());
}

TEST(OpenAddressingHashTableTest, CopyConstructorAndAssignment) {
  OpenAddressingHashTable<int, std::string> t1;
  t1.insert(1, "One");
  t1.insert(2, "Two");

  OpenAddressingHashTable<int, std::string> t2 = t1;
  EXPECT_TRUE(t2.contains(1));
  EXPECT_EQ(t2.size(), 2);

  OpenAddressingHashTable<int, std::string> t3;
  t3 = t1;
  EXPECT_TRUE(t3.contains(2));
}

TEST(OpenAddressingHashTableTest, MoveConstructorAndAssignment) {
  OpenAddressingHashTable<int, std::string> t1;
  t1.insert(1, "One");

  OpenAddressingHashTable<int, std::string> t2 = std::move(t1);
  EXPECT_TRUE(t2.contains(1));
  EXPECT_TRUE(t1.empty());

  OpenAddressingHashTable<int, std::string> t3;
  t3 = std::move(t2);
  EXPECT_TRUE(t3.contains(1));
}

TEST(OpenAddressingHashTableTest, RehashIncreasesCapacity) {
  OpenAddressingHashTable<int, std::string> table;

  for (int i = 0; i < 10; ++i)
    table.insert(i, std::to_string(i));

  size_t old_size = table.get_container().size();
  table.rehash(old_size * 2);
  EXPECT_GE(table.get_container().size(), old_size * 2);
}

TEST(OpenAddressingHashTableTest, EqualityOperator) {
  OpenAddressingHashTable<int, std::string> t1{{1, "a"}, {2, "b"}};
  OpenAddressingHashTable<int, std::string> t2{{1, "a"}, {2, "b"}};
  OpenAddressingHashTable<int, std::string> t3{{1, "x"}};

  EXPECT_TRUE(t1 == t2);
  EXPECT_FALSE(t1 != t2);
  EXPECT_FALSE(t1 == t3);
}

struct Point {
  int x, y;
  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }
};

struct PointHash {
  size_t operator()(const Point &p) const {
    return std::hash<int>()(p.x * 73856093 ^ p.y * 19349663);
  }
};

TEST(OpenAddressingHashTableTest, CustomTypeKeys) {
  OpenAddressingHashTable<Point, std::string, PointHash> table;
  table.insert({1, 2}, "A");
  table.insert({3, 4}, "B");

  EXPECT_TRUE(table.contains({1, 2}));
  EXPECT_EQ(table.find({3, 4})->value, "B");

  table.erase({1, 2});
  EXPECT_FALSE(table.contains({1, 2}));
}

TEST(OpenAddressingHashTableTest, QuadraticProbingCollisions) {
  struct BadHash {
    size_t operator()(int) const { return 1; }
  };

  OpenAddressingHashTable<int, int, BadHash, QuadraticHashing<int>> table;
  for (int i = 0; i < 10; ++i)
    table.insert(i, i * 10);

  for (int i = 0; i < 10; ++i)
    EXPECT_EQ(table.at(i), i * 10);
}

TEST(OpenAddressingHashTableTest, RehashPreservesData) {
  OpenAddressingHashTable<int, std::string> table;

  for (int i = 0; i < 50; ++i)
    table.insert(i, "val" + std::to_string(i));

  table.rehash(128);

  for (int i = 0; i < 50; ++i)
    EXPECT_EQ(table.at(i), "val" + std::to_string(i));
}

TEST(OpenAddressingHashTableTest, LowStressInsertErase) {
  OpenAddressingHashTable<int, int> table;
  const int N = 5000;

  for (int i = 0; i < N; ++i)
    table.insert(i, i);

  EXPECT_EQ(table.size(), N);

  for (int i = 0; i < N; i += 2)
    table.erase(i);

  EXPECT_EQ(table.size(), N / 2);

  for (int i = 0; i < N; ++i)
    if (i % 2 == 0)
      EXPECT_FALSE(table.contains(i));
    else
      EXPECT_TRUE(table.contains(i));
}

TEST(OpenAddressingHashTableTest, HighStressInsertErase) {
  OpenAddressingHashTable<int, int> table;
  const int N = 10000;

  for (int i = 0; i < N; ++i)
    table.insert(i, i);

  EXPECT_EQ(table.size(), N);

  for (int i = 0; i < N; i += 2)
    table.erase(i);

  EXPECT_EQ(table.size(), N / 2);

  for (int i = 0; i < N; ++i)
    if (i % 2 == 0)
      EXPECT_FALSE(table.contains(i));
    else
      EXPECT_TRUE(table.contains(i));
}


TEST(OpenAddressingHashTableTest, SwapWorksCorrectly) {
  OpenAddressingHashTable<int, std::string> t1{{1, "A"}, {2, "B"}};
  OpenAddressingHashTable<int, std::string> t2{{3, "C"}};

  t1.swap(t2);

  EXPECT_TRUE(t1.contains(3));
  EXPECT_FALSE(t1.contains(1));

  EXPECT_TRUE(t2.contains(1));
  EXPECT_TRUE(t2.contains(2));
}

TEST(OpenAddressingHashTableTest, IteratorTraversal) {
  OpenAddressingHashTable<int, std::string> table;
  table.insert(1, "A");
  table.insert(2, "B");
  table.insert(3, "C");

  std::vector<int> keys;
  for (auto &entry : table)
    keys.push_back(entry.key);

  EXPECT_EQ(keys.size(), 3);
  EXPECT_NE(std::find(keys.begin(), keys.end(), 1), keys.end());
}

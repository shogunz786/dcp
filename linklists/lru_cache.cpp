/*
 * LRU (LEAST RECENTLY USED) CACHE IMPLEMENTATION
 *
 * This file implements a high-performance LRU cache using a combination of:
 * - Doubly linked list for maintaining insertion/access order
 * - Hash map for O(1) key lookup
 *
 * ALGORITHM OVERVIEW:
 * ==================
 * LRU cache maintains items in order of recent usage:
 * - Most recently used items are at the front (head)
 * - Least recently used items are at the back (tail)
 * - When cache is full, evict item from tail
 * - When item is accessed, move it to head
 *
 * TIME COMPLEXITY:
 * - GET: O(1) average case
 * - PUT: O(1) average case
 * - All operations are constant time on average
 *
 * SPACE COMPLEXITY:
 * - O(capacity) for storing key-value pairs
 * - O(capacity) for hash map + linked list overhead
 *
 * REAL-WORLD APPLICATIONS:
 * - CPU cache management
 * - Database buffer pools
 * - Web page caching
 * - Operating system page replacement
 * - Application-level caching (Redis, Memcached)
 * - Browser cache management
 *
 * DESIGN PATTERNS:
 * - Template-based for type flexibility
 * - RAII for automatic resource management
 * - Iterator-based design for efficiency
 * - Exception safety guarantees
 */

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <cassert>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;

/**
 * Node structure for the doubly linked list
 *
 * Each node contains:
 * - key: Used for hash map lookups and eviction
 * - value: The actual data being cached
 *
 * Template parameters allow for flexible key/value types
 */
template <typename K, typename V>
struct CacheNode
{
  K key;
  V value;

  // Constructor for easy node creation
  CacheNode(const K &k, const V &v) : key(k), value(v) {}

  // Default constructor for container compatibility
  CacheNode() = default;

  // Copy and move constructors for efficiency
  CacheNode(const CacheNode &) = default;
  CacheNode(CacheNode &&) = default;
  CacheNode &operator=(const CacheNode &) = default;
  CacheNode &operator=(CacheNode &&) = default;
};

/**
 * LRU CACHE IMPLEMENTATION
 *
 * Template class supporting any key-value types that meet requirements:
 * - Key type K must be hashable and equality comparable
 * - Value type V must be copyable/movable
 *
 * INTERNAL STRUCTURE:
 * - std::list<CacheNode<K,V>> for maintaining order (doubly-linked)
 * - std::unordered_map<K, iterator> for O(1) key lookup
 * - Most recent items at front, least recent at back
 *
 * THREAD SAFETY:
 * - Not thread-safe by design (single-threaded optimization)
 * - For multi-threaded use, wrap with mutex or use thread-local instances
 */
template <typename K, typename V>
class LRUCache
{
private:
  // Maximum number of items the cache can hold
  size_t max_capacity;

  // Doubly linked list to maintain insertion/access order
  // Front = most recently used, Back = least recently used
  list<CacheNode<K, V>> cache_list;

  // Hash map for O(1) key lookup, maps key to list iterator
  unordered_map<K, typename list<CacheNode<K, V>>::iterator> key_map;

  // Statistics for monitoring and debugging
  mutable size_t hit_count = 0;      // Number of cache hits
  mutable size_t miss_count = 0;     // Number of cache misses
  mutable size_t eviction_count = 0; // Number of items evicted

  /**
   * Move an existing node to the front of the list (most recently used)
   *
   * @param iter Iterator pointing to the node to promote
   * @return Iterator to the node's new position at front
   *
   * ALGORITHM:
   * 1. Create copy of node at front of list
   * 2. Erase old position
   * 3. Update hash map to point to new position
   * 4. Return new iterator
   *
   * TIME: O(1) - list operations are constant time
   */
  typename list<CacheNode<K, V>>::iterator move_to_front(
      typename list<CacheNode<K, V>>::iterator iter)
  {

    // Create new node at front with same data
    cache_list.push_front(*iter);

    // Remove old position
    cache_list.erase(iter);

    // Return iterator to new front position
    return cache_list.begin();
  }

  /**
   * Evict the least recently used item (from back of list)
   *
   * ALGORITHM:
   * 1. Get key of last item
   * 2. Remove from hash map
   * 3. Remove from list
   * 4. Update eviction statistics
   *
   * TIME: O(1) - all operations are constant time
   */
  void evict_lru()
  {
    if (!cache_list.empty())
    {
      // Get key of least recently used item
      K lru_key = cache_list.back().key;

      // Remove from hash map
      key_map.erase(lru_key);

      // Remove from list
      cache_list.pop_back();

      // Update statistics
      eviction_count++;
    }
  }

public:
  /**
   * Constructor: Initialize LRU cache with specified capacity
   *
   * @param capacity Maximum number of items to store
   * @throws std::invalid_argument if capacity is 0
   */
  explicit LRUCache(size_t capacity) : max_capacity(capacity)
  {
    if (capacity == 0)
    {
      throw std::invalid_argument("Cache capacity must be greater than 0");
    }

    // Reserve space in hash map for efficiency
    key_map.reserve(capacity);
  }

  /**
   * Get value associated with key
   *
   * @param key Key to look up
   * @return Value if found, or default-constructed value if not found
   * @note Updates item's position to most recently used
   *
   * ALGORITHM:
   * 1. Look up key in hash map
   * 2. If found: move to front, return value, increment hit count
   * 3. If not found: increment miss count, return default value
   *
   * TIME: O(1) average case
   * SPACE: O(1)
   */
  V get(const K &key)
  {
    auto map_iter = key_map.find(key);

    if (map_iter == key_map.end())
    {
      // Key not found - cache miss
      miss_count++;
      return V{}; // Return default-constructed value
    }

    // Key found - cache hit
    hit_count++;

    // Move accessed item to front (most recently used)
    auto list_iter = map_iter->second;
    auto new_iter = move_to_front(list_iter);

    // Update hash map to point to new position
    key_map[key] = new_iter;

    // Return the value
    return new_iter->value;
  }

  /**
   * Check if key exists in cache without updating access order
   *
   * @param key Key to check
   * @return true if key exists, false otherwise
   *
   * TIME: O(1) average case
   * SPACE: O(1)
   */
  bool contains(const K &key) const
  {
    return key_map.find(key) != key_map.end();
  }

  /**
   * Insert or update key-value pair
   *
   * @param key Key to insert/update
   * @param value Value to associate with key
   *
   * ALGORITHM:
   * 1. Check if key already exists
   * 2. If exists: update value, move to front
   * 3. If not exists:
   *    a. If at capacity: evict LRU item
   *    b. Insert new item at front
   *    c. Update hash map
   *
   * TIME: O(1) average case
   * SPACE: O(1)
   */
  void put(const K &key, const V &value)
  {
    auto map_iter = key_map.find(key);

    if (map_iter != key_map.end())
    {
      // Key exists - update value and move to front
      auto list_iter = map_iter->second;
      list_iter->value = value; // Update value

      auto new_iter = move_to_front(list_iter);
      key_map[key] = new_iter; // Update hash map
    }
    else
    {
      // Key doesn't exist - insert new item

      // Check if we need to evict (at capacity)
      if (cache_list.size() >= max_capacity)
      {
        evict_lru();
      }

      // Insert new item at front
      cache_list.push_front(CacheNode<K, V>(key, value));

      // Update hash map
      key_map[key] = cache_list.begin();
    }
  }

  /**
   * Remove item from cache
   *
   * @param key Key to remove
   * @return true if item was removed, false if not found
   *
   * TIME: O(1) average case
   * SPACE: O(1)
   */
  bool remove(const K &key)
  {
    auto map_iter = key_map.find(key);

    if (map_iter == key_map.end())
    {
      return false; // Key not found
    }

    // Remove from list
    cache_list.erase(map_iter->second);

    // Remove from hash map
    key_map.erase(map_iter);

    return true;
  }

  /**
   * Clear all items from cache
   *
   * TIME: O(n) where n is current size
   * SPACE: O(1)
   */
  void clear()
  {
    cache_list.clear();
    key_map.clear();

    // Reset statistics
    hit_count = 0;
    miss_count = 0;
    eviction_count = 0;
  }

  /**
   * Get current number of items in cache
   *
   * @return Current cache size
   *
   * TIME: O(1)
   * SPACE: O(1)
   */
  size_t size() const
  {
    return cache_list.size();
  }

  /**
   * Get maximum capacity of cache
   *
   * @return Maximum capacity
   *
   * TIME: O(1)
   * SPACE: O(1)
   */
  size_t capacity() const
  {
    return max_capacity;
  }

  /**
   * Check if cache is empty
   *
   * @return true if empty, false otherwise
   *
   * TIME: O(1)
   * SPACE: O(1)
   */
  bool empty() const
  {
    return cache_list.empty();
  }

  /**
   * Check if cache is at full capacity
   *
   * @return true if full, false otherwise
   *
   * TIME: O(1)
   * SPACE: O(1)
   */
  bool full() const
  {
    return cache_list.size() >= max_capacity;
  }

  /**
   * Get cache hit rate (hits / total accesses)
   *
   * @return Hit rate between 0.0 and 1.0
   *
   * TIME: O(1)
   * SPACE: O(1)
   */
  double hit_rate() const
  {
    size_t total = hit_count + miss_count;
    return total > 0 ? static_cast<double>(hit_count) / total : 0.0;
  }

  /**
   * Get detailed cache statistics
   *
   * @return String with formatted statistics
   */
  string get_stats() const
  {
    ostringstream oss;
    oss << fixed << setprecision(2);
    oss << "Cache Statistics:\n";
    oss << "  Capacity: " << max_capacity << "\n";
    oss << "  Current Size: " << size() << "\n";
    oss << "  Hit Count: " << hit_count << "\n";
    oss << "  Miss Count: " << miss_count << "\n";
    oss << "  Eviction Count: " << eviction_count << "\n";
    oss << "  Hit Rate: " << (hit_rate() * 100) << "%\n";
    return oss.str();
  }

  /**
   * Get keys in LRU order (most recent first)
   *
   * @return Vector of keys in access order
   *
   * TIME: O(n) where n is current size
   * SPACE: O(n)
   */
  vector<K> get_keys() const
  {
    vector<K> keys;
    keys.reserve(cache_list.size());

    for (const auto &node : cache_list)
    {
      keys.push_back(node.key);
    }

    return keys;
  }

  /**
   * Print cache contents in LRU order
   *
   * @param max_items Maximum number of items to print (0 = all)
   */
  void print_cache(size_t max_items = 0) const
  {
    cout << "LRU Cache Contents (most recent first):\n";
    cout << "Size: " << size() << "/" << capacity() << "\n";

    if (empty())
    {
      cout << "(empty)\n";
      return;
    }

    size_t count = 0;
    size_t limit = (max_items == 0) ? cache_list.size() : max_items;

    for (const auto &node : cache_list)
    {
      if (count >= limit)
        break;

      cout << "  [" << count << "] " << node.key
           << " -> " << node.value << "\n";
      count++;
    }

    if (max_items > 0 && cache_list.size() > max_items)
    {
      cout << "  ... (" << (cache_list.size() - max_items)
           << " more items)\n";
    }
  }

  /**
   * Validate internal consistency (for debugging)
   *
   * @return true if consistent, false otherwise
   * @throws std::runtime_error if major inconsistency found
   */
  bool validate() const
  {
    // Check size consistency
    if (cache_list.size() != key_map.size())
    {
      throw std::runtime_error("Size mismatch between list and map");
    }

    // Check capacity constraint
    if (cache_list.size() > max_capacity)
    {
      throw std::runtime_error("Cache size exceeds capacity");
    }

    // Check that every list item has corresponding map entry
    for (auto list_iter = cache_list.begin(); list_iter != cache_list.end(); ++list_iter)
    {
      auto map_iter = key_map.find(list_iter->key);
      if (map_iter == key_map.end())
      {
        throw std::runtime_error("List item not found in map");
      }
      if (map_iter->second != list_iter)
      {
        throw std::runtime_error("Map iterator doesn't match list iterator");
      }
    }

    // Check that every map entry has corresponding list item
    for (const auto &map_pair : key_map)
    {
      if (map_pair.second->key != map_pair.first)
      {
        throw std::runtime_error("Map key doesn't match list item key");
      }
    }

    return true;
  }
};

/**
 * Specialized string-based LRU cache for common use cases
 */
using StringLRUCache = LRUCache<string, string>;
using IntLRUCache = LRUCache<int, int>;

/**
 * Test framework for comprehensive validation
 */
class LRUCacheTestSuite
{
private:
  int tests_passed = 0;
  int tests_failed = 0;
  int total_tests = 0;

  void assert_test(bool condition, const string &test_name, const string &message = "")
  {
    total_tests++;
    if (condition)
    {
      tests_passed++;
      cout << "✅ " << test_name << endl;
    }
    else
    {
      tests_failed++;
      cout << "❌ " << test_name;
      if (!message.empty())
      {
        cout << " - " << message;
      }
      cout << endl;
    }
  }

public:
  void run_all_tests()
  {
    cout << "🧪 LRU CACHE COMPREHENSIVE TEST SUITE\n";
    cout << "====================================\n\n";

    test_basic_operations();
    test_lru_behavior();
    test_capacity_management();
    test_edge_cases();
    test_performance();
    test_different_types();
    test_statistics_tracking();
    test_error_conditions();

    print_summary();
  }

private:
  void test_basic_operations()
  {
    cout << "📋 Testing Basic Operations\n";
    cout << "---------------------------\n";

    IntLRUCache cache(3);

    // Test empty cache
    assert_test(cache.empty(), "Empty cache check");
    assert_test(cache.size() == 0, "Initial size is 0");
    assert_test(cache.capacity() == 3, "Capacity is correct");
    assert_test(!cache.full(), "Cache not full initially");

    // Test insertion
    cache.put(1, 10);
    assert_test(!cache.empty(), "Cache not empty after insertion");
    assert_test(cache.size() == 1, "Size is 1 after one insertion");
    assert_test(cache.contains(1), "Contains inserted key");

    // Test retrieval
    int value = cache.get(1);
    assert_test(value == 10, "Retrieved correct value");

    // Test non-existent key
    int missing = cache.get(999);
    assert_test(missing == 0, "Non-existent key returns default value");
    assert_test(!cache.contains(999), "Non-existent key not contained");

    // Test update
    cache.put(1, 20);
    assert_test(cache.size() == 1, "Size unchanged after update");
    assert_test(cache.get(1) == 20, "Updated value retrieved correctly");

    cout << endl;
  }

  void test_lru_behavior()
  {
    cout << "📋 Testing LRU Behavior\n";
    cout << "------------------------\n";

    IntLRUCache cache(3);

    // Fill cache
    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);

    assert_test(cache.full(), "Cache is full");
    assert_test(cache.size() == 3, "Cache size is at capacity");

    // Verify order (most recent first)
    auto keys = cache.get_keys();
    assert_test(keys[0] == 3 && keys[1] == 2 && keys[2] == 1,
                "Keys in correct LRU order after insertion");

    // Access key 1 (should move to front)
    cache.get(1);
    keys = cache.get_keys();
    assert_test(keys[0] == 1, "Accessed key moved to front");

    // Add new item (should evict key 2)
    cache.put(4, 40);
    assert_test(!cache.contains(2), "LRU item (2) was evicted");
    assert_test(cache.contains(1), "Recently accessed item (1) retained");
    assert_test(cache.contains(3), "Other item (3) retained");
    assert_test(cache.contains(4), "New item (4) added");

    keys = cache.get_keys();
    assert_test(keys[0] == 4, "New item at front");

    cout << endl;
  }

  void test_capacity_management()
  {
    cout << "📋 Testing Capacity Management\n";
    cout << "-------------------------------\n";

    IntLRUCache cache(2);

    // Fill to capacity
    cache.put(1, 10);
    cache.put(2, 20);
    assert_test(cache.full(), "Cache full at capacity");

    // Add beyond capacity
    cache.put(3, 30);
    assert_test(cache.size() == 2, "Size remains at capacity");
    assert_test(!cache.contains(1), "First item evicted");
    assert_test(cache.contains(2), "Second item retained");
    assert_test(cache.contains(3), "New item added");

    // Test removal
    bool removed = cache.remove(2);
    assert_test(removed, "Item removal successful");
    assert_test(cache.size() == 1, "Size decreased after removal");
    assert_test(!cache.contains(2), "Removed item not found");

    // Test removal of non-existent item
    bool not_removed = cache.remove(999);
    assert_test(!not_removed, "Non-existent item removal returns false");

    // Test clear
    cache.clear();
    assert_test(cache.empty(), "Cache empty after clear");
    assert_test(cache.size() == 0, "Size is 0 after clear");

    cout << endl;
  }

  void test_edge_cases()
  {
    cout << "📋 Testing Edge Cases\n";
    cout << "----------------------\n";

    // Test single-item cache
    IntLRUCache small_cache(1);
    small_cache.put(1, 10);
    small_cache.put(2, 20);
    assert_test(small_cache.size() == 1, "Single-item cache maintains size");
    assert_test(!small_cache.contains(1), "First item evicted in single-item cache");
    assert_test(small_cache.contains(2), "Second item retained in single-item cache");

    // Test repeated operations
    IntLRUCache cache(3);
    for (int i = 0; i < 10; i++)
    {
      cache.put(1, i);
    }
    assert_test(cache.size() == 1, "Repeated puts don't increase size");
    assert_test(cache.get(1) == 9, "Latest value retained");

    // Test alternating access pattern
    cache.clear();
    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);

    for (int i = 0; i < 5; i++)
    {
      cache.get(1);
      cache.get(2);
    }

    cache.put(4, 40); // Should evict 3
    assert_test(!cache.contains(3), "Least accessed item evicted");
    assert_test(cache.contains(1), "Frequently accessed item retained");
    assert_test(cache.contains(2), "Frequently accessed item retained");

    cout << endl;
  }

  void test_performance()
  {
    cout << "📋 Testing Performance\n";
    cout << "-----------------------\n";

    const int cache_size = 1000;
    const int num_operations = 10000;
    IntLRUCache cache(cache_size);

    auto start = high_resolution_clock::now();

    // Random operations
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> key_dist(1, num_operations * 2);
    uniform_int_distribution<> op_dist(0, 1);

    for (int i = 0; i < num_operations; i++)
    {
      int key = key_dist(gen);
      if (op_dist(gen) == 0)
      {
        cache.put(key, key * 10);
      }
      else
      {
        cache.get(key);
      }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    double ops_per_second = (num_operations * 1000000.0) / duration.count();

    cout << "Performance test: " << num_operations << " operations in "
         << duration.count() << " microseconds\n";
    cout << "Operations per second: " << fixed << setprecision(0)
         << ops_per_second << "\n";

    assert_test(ops_per_second > 100000, "Performance meets minimum threshold");

    cout << endl;
  }

  void test_different_types()
  {
    cout << "📋 Testing Different Types\n";
    cout << "---------------------------\n";

    // String cache test
    StringLRUCache str_cache(2);
    str_cache.put("key1", "value1");
    str_cache.put("key2", "value2");

    assert_test(str_cache.get("key1") == "value1", "String cache stores strings correctly");
    assert_test(str_cache.contains("key2"), "String cache contains check works");

    str_cache.put("key3", "value3");
    assert_test(!str_cache.contains("key1"), "String cache evicts correctly");

    // Custom type test with pairs
    LRUCache<int, pair<string, int>> custom_cache(2);
    custom_cache.put(1, make_pair("first", 100));
    custom_cache.put(2, make_pair("second", 200));

    auto result = custom_cache.get(1);
    assert_test(result.first == "first" && result.second == 100,
                "Custom type cache works correctly");

    cout << endl;
  }

  void test_statistics_tracking()
  {
    cout << "📋 Testing Statistics Tracking\n";
    cout << "-------------------------------\n";

    IntLRUCache cache(3);

    // Generate some hits and misses
    cache.put(1, 10);
    cache.put(2, 20);

    cache.get(1); // hit
    cache.get(2); // hit
    cache.get(3); // miss
    cache.get(4); // miss

    assert_test(cache.hit_rate() == 0.5, "Hit rate calculated correctly");

    cache.put(3, 30);
    cache.put(4, 40); // Should cause eviction

    string stats = cache.get_stats();
    assert_test(!stats.empty(), "Statistics string generated");
    assert_test(stats.find("Hit Rate") != string::npos, "Hit rate in statistics");

    cout << endl;
  }

  void test_error_conditions()
  {
    cout << "📋 Testing Error Conditions\n";
    cout << "----------------------------\n";

    bool exception_caught = false;

    try
    {
      IntLRUCache invalid_cache(0); // Should throw
    }
    catch (const std::invalid_argument &)
    {
      exception_caught = true;
    }

    assert_test(exception_caught, "Zero capacity throws exception");

    // Test validation
    IntLRUCache cache(3);
    cache.put(1, 10);
    cache.put(2, 20);

    bool validation_passed = false;
    try
    {
      validation_passed = cache.validate();
    }
    catch (const exception &)
    {
      validation_passed = false;
    }

    assert_test(validation_passed, "Cache validation passes for valid cache");

    cout << endl;
  }

  void print_summary()
  {
    cout << "🎉 TEST SUMMARY\n";
    cout << "===============\n";
    cout << "Total tests: " << total_tests << "\n";
    cout << "✅ Passed: " << tests_passed << "\n";
    cout << "❌ Failed: " << tests_failed << "\n";
    cout << "Success rate: " << fixed << setprecision(1)
         << (100.0 * tests_passed / total_tests) << "%\n\n";

    if (tests_failed == 0)
    {
      cout << "🎊 ALL TESTS PASSED! LRU Cache implementation is working correctly.\n";
    }
    else
    {
      cout << "⚠️  Some tests failed. Check implementation for issues.\n";
    }
  }
};

/**
 * Interactive demonstration of LRU cache behavior
 */
void run_interactive_demo()
{
  cout << "🎮 INTERACTIVE LRU CACHE DEMONSTRATION\n";
  cout << "======================================\n\n";

  IntLRUCache cache(4); // Small cache for demonstration

  cout << "Creating LRU cache with capacity 4...\n\n";

  // Demonstrate basic operations
  cout << "📝 Step 1: Adding items to cache\n";
  cache.put(1, 100);
  cout << "Added: 1 -> 100\n";
  cache.print_cache();
  cout << "\n";

  cache.put(2, 200);
  cout << "Added: 2 -> 200\n";
  cache.print_cache();
  cout << "\n";

  cache.put(3, 300);
  cache.put(4, 400);
  cout << "Added: 3 -> 300, 4 -> 400\n";
  cache.print_cache();
  cout << "\n";

  // Demonstrate LRU behavior
  cout << "📝 Step 2: Accessing existing items (moves to front)\n";
  cout << "Accessing key 2...\n";
  int value = cache.get(2);
  cout << "Retrieved: " << value << "\n";
  cache.print_cache();
  cout << "\n";

  // Demonstrate eviction
  cout << "📝 Step 3: Adding beyond capacity (triggers eviction)\n";
  cache.put(5, 500);
  cout << "Added: 5 -> 500 (should evict least recently used)\n";
  cache.print_cache();
  cout << "\n";

  // Show statistics
  cout << "📊 Cache Statistics:\n";
  cout << cache.get_stats() << "\n";
}

/**
 * Benchmark different cache sizes and access patterns
 */
void run_performance_benchmark()
{
  cout << "⚡ PERFORMANCE BENCHMARK\n";
  cout << "========================\n\n";

  vector<int> cache_sizes = {10, 100, 1000, 10000};
  const int num_operations = 100000;

  cout << "Running " << num_operations << " operations for different cache sizes:\n\n";

  for (int cache_size : cache_sizes)
  {
    IntLRUCache cache(cache_size);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> key_dist(1, cache_size * 2);
    uniform_int_distribution<> op_dist(0, 2); // 0=get, 1=put, 2=get

    auto start = high_resolution_clock::now();

    for (int i = 0; i < num_operations; i++)
    {
      int key = key_dist(gen);
      if (op_dist(gen) == 1)
      {
        cache.put(key, key * 10);
      }
      else
      {
        cache.get(key);
      }
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    double ops_per_second = (num_operations * 1000000.0) / duration.count();

    cout << "Cache size: " << setw(6) << cache_size
         << " | Operations/sec: " << setw(10) << fixed << setprecision(0)
         << ops_per_second
         << " | Hit rate: " << setw(5) << setprecision(1)
         << (cache.hit_rate() * 100) << "%"
         << " | Final size: " << cache.size() << "\n";
  }

  cout << endl;
}

/**
 * MAIN FUNCTION WITH COMPREHENSIVE TESTING AND DEMONSTRATIONS
 */
int main(int argc, char *argv[])
{
  cout << "🚀 LRU CACHE IMPLEMENTATION - COMPREHENSIVE TESTING\n";
  cout << "===================================================\n\n";

  // Parse command line arguments
  bool run_tests = true;
  bool run_demo = false;
  bool run_benchmark = false;

  for (int i = 1; i < argc; i++)
  {
    string arg = argv[i];
    if (arg == "--demo" || arg == "-d")
    {
      run_demo = true;
    }
    else if (arg == "--benchmark" || arg == "-b")
    {
      run_benchmark = true;
    }
    else if (arg == "--no-tests" || arg == "-n")
    {
      run_tests = false;
    }
    else if (arg == "--help" || arg == "-h")
    {
      cout << "LRU Cache Test Program\n";
      cout << "Usage: " << argv[0] << " [options]\n";
      cout << "Options:\n";
      cout << "  -d, --demo        Run interactive demonstration\n";
      cout << "  -b, --benchmark   Run performance benchmark\n";
      cout << "  -n, --no-tests    Skip automated tests\n";
      cout << "  -h, --help        Show this help\n";
      return 0;
    }
  }

  int exit_code = 0;

  // Run comprehensive tests
  if (run_tests)
  {
    LRUCacheTestSuite test_suite;
    test_suite.run_all_tests();
    cout << endl;
  }

  // Run interactive demonstration
  if (run_demo)
  {
    run_interactive_demo();
    cout << endl;
  }

  // Run performance benchmark
  if (run_benchmark)
  {
    run_performance_benchmark();
    cout << endl;
  }

  // If no specific options, run everything
  if (!run_demo && !run_benchmark && run_tests)
  {
    cout << "🎯 QUICK DEMONSTRATION\n";
    cout << "======================\n\n";

    // Quick example
    StringLRUCache demo_cache(3);

    cout << "Creating string-based LRU cache with capacity 3:\n";
    demo_cache.put("apple", "red fruit");
    demo_cache.put("banana", "yellow fruit");
    demo_cache.put("cherry", "red small fruit");

    cout << "\nCache contents:\n";
    demo_cache.print_cache();

    cout << "\nAccessing 'apple' (moves to front):\n";
    string result = demo_cache.get("apple");
    cout << "Retrieved: " << result << "\n";
    demo_cache.print_cache();

    cout << "\nAdding 'date' (should evict 'banana'):\n";
    demo_cache.put("date", "sweet brown fruit");
    demo_cache.print_cache();

    cout << "\n"
         << demo_cache.get_stats() << "\n";
  }

  cout << "🎓 LRU CACHE CONCEPTS DEMONSTRATED:\n";
  cout << "===================================\n";
  cout << "✅ O(1) get and put operations\n";
  cout << "✅ Automatic eviction of least recently used items\n";
  cout << "✅ Template-based design for any key-value types\n";
  cout << "✅ Comprehensive statistics tracking\n";
  cout << "✅ Memory-efficient doubly-linked list + hash map\n";
  cout << "✅ Exception safety and error handling\n";
  cout << "✅ Performance optimization for high-throughput scenarios\n";

  cout << "\n🔧 COMPILATION:\n";
  cout << "   g++ -std=c++11 -O2 -o lru_cache lru_cache.cpp\n";
  cout << "   ./lru_cache --demo --benchmark\n";

  return exit_code;
}

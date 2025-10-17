/*
 * LRU CACHE HEADER FILE
 *
 * Template-based LRU (Least Recently Used) cache implementation
 * with O(1) get and put operations.
 */

#ifndef LRU_CACHE_H
#define LRU_CACHE_H

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

/**
 * Node structure for the doubly linked list
 */
template <typename K, typename V>
struct CacheNode
{
    K key;
    V value;

    CacheNode(const K &k, const V &v) : key(k), value(v) {}
    CacheNode() = default;
    CacheNode(const CacheNode &) = default;
    CacheNode(CacheNode &&) = default;
    CacheNode &operator=(const CacheNode &) = default;
    CacheNode &operator=(CacheNode &&) = default;
};

/**
 * LRU CACHE IMPLEMENTATION
 */
template <typename K, typename V>
class LRUCache
{
private:
    size_t max_capacity;
    list<CacheNode<K, V>> cache_list;
    unordered_map<K, typename list<CacheNode<K, V>>::iterator> key_map;

    // Statistics
    mutable size_t hit_count = 0;
    mutable size_t miss_count = 0;
    mutable size_t eviction_count = 0;

    typename list<CacheNode<K, V>>::iterator move_to_front(
        typename list<CacheNode<K, V>>::iterator iter)
    {
        cache_list.push_front(*iter);
        cache_list.erase(iter);
        return cache_list.begin();
    }

    void evict_lru()
    {
        if (!cache_list.empty())
        {
            K lru_key = cache_list.back().key;
            key_map.erase(lru_key);
            cache_list.pop_back();
            eviction_count++;
        }
    }

public:
    explicit LRUCache(size_t capacity) : max_capacity(capacity)
    {
        if (capacity == 0)
        {
            throw std::invalid_argument("Cache capacity must be greater than 0");
        }
        key_map.reserve(capacity);
    }

    V get(const K &key)
    {
        auto map_iter = key_map.find(key);

        if (map_iter == key_map.end())
        {
            miss_count++;
            return V{};
        }

        hit_count++;
        auto list_iter = map_iter->second;
        auto new_iter = move_to_front(list_iter);
        key_map[key] = new_iter;
        return new_iter->value;
    }

    bool contains(const K &key) const
    {
        return key_map.find(key) != key_map.end();
    }

    void put(const K &key, const V &value)
    {
        auto map_iter = key_map.find(key);

        if (map_iter != key_map.end())
        {
            auto list_iter = map_iter->second;
            list_iter->value = value;
            auto new_iter = move_to_front(list_iter);
            key_map[key] = new_iter;
        }
        else
        {
            if (cache_list.size() >= max_capacity)
            {
                evict_lru();
            }
            cache_list.push_front(CacheNode<K, V>(key, value));
            key_map[key] = cache_list.begin();
        }
    }

    bool remove(const K &key)
    {
        auto map_iter = key_map.find(key);
        if (map_iter == key_map.end())
        {
            return false;
        }
        cache_list.erase(map_iter->second);
        key_map.erase(map_iter);
        return true;
    }

    void clear()
    {
        cache_list.clear();
        key_map.clear();
        hit_count = 0;
        miss_count = 0;
        eviction_count = 0;
    }

    size_t size() const { return cache_list.size(); }
    size_t capacity() const { return max_capacity; }
    bool empty() const { return cache_list.empty(); }
    bool full() const { return cache_list.size() >= max_capacity; }

    double hit_rate() const
    {
        size_t total = hit_count + miss_count;
        return total > 0 ? static_cast<double>(hit_count) / total : 0.0;
    }

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

    bool validate() const
    {
        if (cache_list.size() != key_map.size())
        {
            throw std::runtime_error("Size mismatch between list and map");
        }
        if (cache_list.size() > max_capacity)
        {
            throw std::runtime_error("Cache size exceeds capacity");
        }
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

// Convenience type aliases
using StringLRUCache = LRUCache<string, string>;
using IntLRUCache = LRUCache<int, int>;

#endif // LRU_CACHE_H
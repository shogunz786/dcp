/*
 * LRU CACHE USAGE EXAMPLES AND PATTERNS
 *
 * This file demonstrates various real-world usage patterns for the LRU cache
 * implementation, showing how it can be applied in different scenarios.
 */

#include "lru_cache.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

/**
 * Example 1: Web Page Caching
 * Simulates a web server caching rendered pages
 */
void example_web_cache()
{
    cout << "🌐 Web Page Caching Example\n";
    cout << "============================\n";

    // Cache for rendered web pages
    LRUCache<string, string> page_cache(5);

    // Simulate page rendering and caching
    auto render_page = [&](const string &url) -> string
    {
        if (page_cache.contains(url))
        {
            cout << "📄 Cache HIT for " << url << "\n";
            return page_cache.get(url);
        }
        else
        {
            cout << "🔄 Cache MISS for " << url << " - rendering...\n";
            // Simulate expensive page rendering
            this_thread::sleep_for(chrono::milliseconds(100));
            string content = "<html><h1>Page: " + url + "</h1></html>";
            page_cache.put(url, content);
            return content;
        }
    };

    // Simulate web requests
    vector<string> requests = {
        "/home", "/about", "/products", "/contact", "/blog",
        "/home",     // Should be cache hit
        "/services", // Should evict oldest
        "/about",    // Should be cache hit
        "/news"      // Should evict another
    };

    for (const auto &url : requests)
    {
        string page = render_page(url);
        cout << "   Content length: " << page.length() << " chars\n";
    }

    cout << "\n"
         << page_cache.get_stats() << "\n";
}

/**
 * Example 2: Database Connection Pool
 * Manages database connections with LRU eviction
 */
class DatabaseConnection
{
public:
    string connection_string;
    bool active;

    DatabaseConnection(const string &conn_str)
        : connection_string(conn_str), active(true)
    {
        cout << "📡 Created connection: " << conn_str << "\n";
    }

    ~DatabaseConnection()
    {
        cout << "❌ Destroyed connection: " << connection_string << "\n";
    }

    void query(const string &sql)
    {
        cout << "🔍 Executing: " << sql << " on " << connection_string << "\n";
    }
};

void example_connection_pool()
{
    cout << "🗄️  Database Connection Pool Example\n";
    cout << "====================================\n";

    // Cache for database connections (using smart pointers for safety)
    LRUCache<string, shared_ptr<DatabaseConnection>> conn_pool(3);

    auto get_connection = [&](const string &db_name) -> shared_ptr<DatabaseConnection>
    {
        if (conn_pool.contains(db_name))
        {
            cout << "♻️  Reusing connection for " << db_name << "\n";
            return conn_pool.get(db_name);
        }
        else
        {
            cout << "🆕 Creating new connection for " << db_name << "\n";
            auto conn = make_shared<DatabaseConnection>("jdbc:mysql://" + db_name);
            conn_pool.put(db_name, conn);
            return conn;
        }
    };

    // Simulate database operations
    auto conn1 = get_connection("users_db");
    conn1->query("SELECT * FROM users");

    auto conn2 = get_connection("products_db");
    conn2->query("SELECT * FROM products");

    auto conn3 = get_connection("orders_db");
    conn3->query("SELECT * FROM orders");

    // Reuse existing connection
    auto conn1_reuse = get_connection("users_db");
    conn1_reuse->query("UPDATE users SET last_login = NOW()");

    // This should evict the least recently used connection
    auto conn4 = get_connection("analytics_db");
    conn4->query("SELECT COUNT(*) FROM events");

    cout << "\n"
         << conn_pool.get_stats() << "\n";
}

/**
 * Example 3: Computational Result Caching
 * Caches expensive mathematical computations
 */
void example_computation_cache()
{
    cout << "🧮 Computational Result Caching Example\n";
    cout << "=======================================\n";

    // Cache for expensive computations
    LRUCache<int, double> fibonacci_cache(10);

    function<double(int)> fibonacci = [&](int n) -> double
    {
        if (n <= 1)
            return n;

        if (fibonacci_cache.contains(n))
        {
            cout << "📊 Cache hit for fibonacci(" << n << ")\n";
            return fibonacci_cache.get(n);
        }

        cout << "🔢 Computing fibonacci(" << n << ")...\n";
        // Simulate expensive computation
        this_thread::sleep_for(chrono::milliseconds(50));

        double result = fibonacci(n - 1) + fibonacci(n - 2);
        fibonacci_cache.put(n, result);
        return result;
    };

    // Compute some Fibonacci numbers
    vector<int> numbers = {10, 8, 12, 8, 15, 10, 20, 12};

    for (int n : numbers)
    {
        auto start = chrono::high_resolution_clock::now();
        double result = fibonacci(n);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

        cout << "   fibonacci(" << n << ") = " << result
             << " (computed in " << duration.count() << "μs)\n";
    }

    cout << "\n"
         << fibonacci_cache.get_stats() << "\n";
}

/**
 * Example 4: Image Processing Cache
 * Caches processed images to avoid recomputation
 */
struct ProcessedImage
{
    string filename;
    int width, height;
    string filters_applied;

    ProcessedImage() : width(0), height(0) {}

    ProcessedImage(const string &name, int w, int h, const string &filters)
        : filename(name), width(w), height(h), filters_applied(filters) {}

    bool empty() const { return width == 0 || height == 0; }
};

void example_image_cache()
{
    cout << "🖼️  Image Processing Cache Example\n";
    cout << "=================================\n";

    // Cache for processed images
    LRUCache<string, ProcessedImage> image_cache(4);

    auto process_image = [&](const string &filename, const string &filters) -> ProcessedImage
    {
        string cache_key = filename + "|" + filters;

        if (image_cache.contains(cache_key))
        {
            cout << "🖼️  Using cached image: " << filename << " with filters: " << filters << "\n";
            return image_cache.get(cache_key);
        }

        cout << "🎨 Processing image: " << filename << " with filters: " << filters << "\n";
        // Simulate expensive image processing
        this_thread::sleep_for(chrono::milliseconds(200));

        ProcessedImage result(filename, 1920, 1080, filters);
        image_cache.put(cache_key, result);
        return result;
    };

    // Simulate image processing requests
    vector<pair<string, string>> requests = {
        {"photo1.jpg", "blur,resize"},
        {"photo2.jpg", "sharpen,crop"},
        {"photo1.jpg", "blur,resize"}, // Cache hit
        {"photo3.jpg", "sepia,rotate"},
        {"photo4.jpg", "contrast,brightness"},
        {"photo2.jpg", "sharpen,crop"},     // Cache hit
        {"photo5.jpg", "vintage,vignette"}, // Should evict oldest
        {"photo1.jpg", "blur,resize"}       // Should be evicted, cache miss
    };

    for (const auto &req : requests)
    {
        auto start = chrono::high_resolution_clock::now();
        ProcessedImage img = process_image(req.first, req.second);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

        cout << "   Result: " << img.width << "x" << img.height
             << " (" << duration.count() << "ms)\n";
    }

    cout << "\n"
         << image_cache.get_stats() << "\n";
}

/**
 * Example 5: Multi-level Cache System
 * Demonstrates hierarchical caching with different policies
 */
void example_multilevel_cache()
{
    cout << "🏗️  Multi-level Cache System Example\n";
    cout << "===================================\n";

    // L1 Cache: Small, fast
    LRUCache<string, string> l1_cache(2);

    // L2 Cache: Larger, slower
    LRUCache<string, string> l2_cache(5);

    auto get_data = [&](const string &key) -> string
    {
        // Try L1 cache first
        if (l1_cache.contains(key))
        {
            cout << "🏃 L1 Cache HIT for " << key << "\n";
            return l1_cache.get(key);
        }

        // Try L2 cache
        if (l2_cache.contains(key))
        {
            cout << "🚶 L2 Cache HIT for " << key << "\n";
            string value = l2_cache.get(key);
            l1_cache.put(key, value); // Promote to L1
            return value;
        }

        // Cache miss - simulate expensive data fetch
        cout << "🐌 Cache MISS for " << key << " - fetching from storage...\n";
        this_thread::sleep_for(chrono::milliseconds(100));
        string value = "data_" + key;

        // Store in both caches
        l2_cache.put(key, value);
        l1_cache.put(key, value);

        return value;
    };

    // Simulate data access pattern
    vector<string> keys = {"A", "B", "C", "A", "D", "B", "E", "A", "F", "C"};

    for (const string &key : keys)
    {
        string data = get_data(key);
        cout << "   Retrieved: " << data << "\n";
    }

    cout << "\nL1 " << l1_cache.get_stats();
    cout << "L2 " << l2_cache.get_stats() << "\n";
}

/**
 * Main function demonstrating all examples
 */
int main()
{
    cout << "🎯 LRU CACHE USAGE EXAMPLES AND PATTERNS\n";
    cout << "========================================\n\n";

    example_web_cache();
    cout << "\n"
         << string(60, '-') << "\n\n";

    example_connection_pool();
    cout << "\n"
         << string(60, '-') << "\n\n";

    example_computation_cache();
    cout << "\n"
         << string(60, '-') << "\n\n";

    example_image_cache();
    cout << "\n"
         << string(60, '-') << "\n\n";

    example_multilevel_cache();

    cout << "\n🎊 All examples completed successfully!\n";
    cout << "\n🎓 Key Takeaways:\n";
    cout << "=================\n";
    cout << "✅ LRU cache improves performance by avoiding repeated expensive operations\n";
    cout << "✅ Template design allows caching any key-value types\n";
    cout << "✅ Statistics help monitor cache effectiveness\n";
    cout << "✅ Multi-level caching can optimize memory hierarchy\n";
    cout << "✅ Smart pointer usage ensures proper resource management\n";
    cout << "✅ Cache keys should encode all relevant parameters\n";

    return 0;
}
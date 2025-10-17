/*
 * SIMPLIFIED DEADLOCK DEMONSTRATION
 * 
 * A more controlled demonstration of deadlock scenarios and prevention techniques
 * that won't hang during testing. This version uses timeouts and careful timing
 * to show deadlock concepts without actually blocking forever.
 *
 * CONCEPTS DEMONSTRATED:
 * 1. Classic deadlock setup (with timeout to prevent hanging)
 * 2. Lock ordering prevention
 * 3. Timeout-based prevention
 * 4. RAII lock management
 * 5. Comprehensive statistics and monitoring
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
#include <cassert>

using namespace std;
using namespace std::chrono;

// Global statistics
atomic<int> successful_operations(0);
atomic<int> deadlock_scenarios(0);
atomic<int> timeout_preventions(0);

/**
 * Thread-safe mutex wrapper with timeout support
 */
class SafeMutex {
private:
    timed_mutex mtx;
    string name;
    atomic<bool> is_locked{false};
    
public:
    SafeMutex(const string& mutex_name) : name(mutex_name) {}
    
    void lock() {
        cout << "[" << this_thread::get_id() << "] Acquiring " << name << endl;
        mtx.lock();
        is_locked = true;
        cout << "[" << this_thread::get_id() << "] ✓ Got " << name << endl;
    }
    
    void unlock() {
        cout << "[" << this_thread::get_id() << "] Releasing " << name << endl;
        is_locked = false;
        mtx.unlock();
    }
    
    bool try_lock_for(int timeout_ms) {
        cout << "[" << this_thread::get_id() << "] Trying " << name 
             << " (timeout: " << timeout_ms << "ms)" << endl;
        
        if (mtx.try_lock_for(chrono::milliseconds(timeout_ms))) {
            is_locked = true;
            cout << "[" << this_thread::get_id() << "] ✓ Got " << name 
                 << " within timeout" << endl;
            return true;
        } else {
            cout << "[" << this_thread::get_id() << "] ⏰ Timeout on " 
                 << name << endl;
            return false;
        }
    }
    
    const string& get_name() const { return name; }
    bool locked() const { return is_locked.load(); }
};

// Global mutexes
SafeMutex mutex_a("A");
SafeMutex mutex_b("B");

/**
 * TEST 1: Deadlock-Prone Scenario (with timeout safety)
 * Shows what would deadlock but uses timeouts to prevent hanging
 */
void deadlock_prone_thread_1() {
    cout << "\n🔴 DeadlockProne Thread 1: Will try A→B" << endl;
    
    mutex_a.lock();
    this_thread::sleep_for(milliseconds(100));  // Give other thread time
    
    cout << "[Thread1] Now trying to get B..." << endl;
    if (mutex_b.try_lock_for(500)) {  // Timeout prevents infinite wait
        cout << "[Thread1] ✅ Got both locks! Working..." << endl;
        this_thread::sleep_for(milliseconds(50));
        mutex_b.unlock();
        successful_operations++;
    } else {
        cout << "[Thread1] ❌ Would have deadlocked! (timeout saved us)" << endl;
        deadlock_scenarios++;
    }
    
    mutex_a.unlock();
    cout << "🔴 DeadlockProne Thread 1 finished" << endl;
}

void deadlock_prone_thread_2() {
    cout << "\n🔴 DeadlockProne Thread 2: Will try B→A" << endl;
    
    mutex_b.lock();
    this_thread::sleep_for(milliseconds(100));  // Give other thread time
    
    cout << "[Thread2] Now trying to get A..." << endl;
    if (mutex_a.try_lock_for(500)) {  // Timeout prevents infinite wait
        cout << "[Thread2] ✅ Got both locks! Working..." << endl;
        this_thread::sleep_for(milliseconds(50));
        mutex_a.unlock();
        successful_operations++;
    } else {
        cout << "[Thread2] ❌ Would have deadlocked! (timeout saved us)" << endl;
        deadlock_scenarios++;
    }
    
    mutex_b.unlock();
    cout << "🔴 DeadlockProne Thread 2 finished" << endl;
}

/**
 * TEST 2: Lock Ordering Prevention
 * Both threads acquire locks in same order: A then B
 */
void ordered_thread_1() {
    cout << "\n🟢 OrderedThread 1: Using A→B order" << endl;
    
    mutex_a.lock();
    cout << "[OrderedThread1] Got A, brief pause..." << endl;
    this_thread::sleep_for(milliseconds(50));
    
    mutex_b.lock();
    cout << "[OrderedThread1] ✅ Got both A and B! Working..." << endl;
    this_thread::sleep_for(milliseconds(100));
    
    mutex_b.unlock();
    mutex_a.unlock();
    successful_operations++;
    cout << "🟢 OrderedThread 1 finished successfully" << endl;
}

void ordered_thread_2() {
    cout << "\n🟢 OrderedThread 2: Using same A→B order" << endl;
    
    // Small initial delay so threads don't start exactly together
    this_thread::sleep_for(milliseconds(25));
    
    mutex_a.lock();
    cout << "[OrderedThread2] Got A, brief pause..." << endl;
    this_thread::sleep_for(milliseconds(50));
    
    mutex_b.lock();
    cout << "[OrderedThread2] ✅ Got both A and B! Working..." << endl;
    this_thread::sleep_for(milliseconds(100));
    
    mutex_b.unlock();
    mutex_a.unlock();
    successful_operations++;
    cout << "🟢 OrderedThread 2 finished successfully" << endl;
}

/**
 * TEST 3: Timeout Strategy
 * Aggressive timeout handling to prevent deadlocks
 */
void timeout_thread_1() {
    cout << "\n🟡 TimeoutThread 1: A→B with aggressive timeouts" << endl;
    
    mutex_a.lock();
    
    if (mutex_b.try_lock_for(200)) {  // Short timeout
        cout << "[TimeoutThread1] ✅ Got both locks! Working..." << endl;
        this_thread::sleep_for(milliseconds(50));
        mutex_b.unlock();
        successful_operations++;
    } else {
        cout << "[TimeoutThread1] ⏰ Timeout prevented deadlock" << endl;
        timeout_preventions++;
    }
    
    mutex_a.unlock();
    cout << "🟡 TimeoutThread 1 finished" << endl;
}

void timeout_thread_2() {
    cout << "\n🟡 TimeoutThread 2: B→A with aggressive timeouts" << endl;
    
    mutex_b.lock();
    
    if (mutex_a.try_lock_for(200)) {  // Short timeout
        cout << "[TimeoutThread2] ✅ Got both locks! Working..." << endl;
        this_thread::sleep_for(milliseconds(50));
        mutex_a.unlock();
        successful_operations++;
    } else {
        cout << "[TimeoutThread2] ⏰ Timeout prevented deadlock" << endl;
        timeout_preventions++;
    }
    
    mutex_b.unlock();
    cout << "🟡 TimeoutThread 2 finished" << endl;
}

/**
 * TEST 4: RAII Lock Manager
 */
class LockManager {
private:
    vector<SafeMutex*> held_locks;
    
public:
    bool acquire_multiple(vector<SafeMutex*> locks, int timeout_ms = 1000) {
        // Sort locks by name to ensure consistent ordering
        sort(locks.begin(), locks.end(), 
             [](SafeMutex* a, SafeMutex* b) {
                 return a->get_name() < b->get_name();
             });
        
        for (auto* lock : locks) {
            if (lock->try_lock_for(timeout_ms / locks.size())) {
                held_locks.push_back(lock);
            } else {
                release_all();
                return false;
            }
        }
        return true;
    }
    
    void release_all() {
        for (auto it = held_locks.rbegin(); it != held_locks.rend(); ++it) {
            (*it)->unlock();
        }
        held_locks.clear();
    }
    
    ~LockManager() {
        release_all();
    }
};

void raii_thread_example(int id) {
    cout << "\n🔵 RAII Thread " << id << ": Using automatic lock management" << endl;
    
    LockManager manager;
    vector<SafeMutex*> needed = {&mutex_a, &mutex_b};
    
    if (manager.acquire_multiple(needed, 300)) {
        cout << "[RAII" << id << "] ✅ Got all locks! Working..." << endl;
        this_thread::sleep_for(milliseconds(100));
        successful_operations++;
    } else {
        cout << "[RAII" << id << "] ❌ Failed to get all locks" << endl;
        timeout_preventions++;
    }
    
    // Locks automatically released by destructor
    cout << "🔵 RAII Thread " << id << " finished" << endl;
}

/**
 * Run a specific test with timing and statistics
 */
void run_test(const string& test_name, function<void()> test_func) {
    cout << "\n" << string(60, '=') << endl;
    cout << "🧪 " << test_name << endl;
    cout << string(60, '=') << endl;
    
    // Reset counters
    int before_success = successful_operations.load();
    int before_deadlock = deadlock_scenarios.load();
    int before_timeout = timeout_preventions.load();
    
    auto start = high_resolution_clock::now();
    test_func();
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<milliseconds>(end - start);
    
    cout << "\n📊 Test Results:" << endl;
    cout << "   ⏱️  Duration: " << duration.count() << "ms" << endl;
    cout << "   ✅ Success: " << (successful_operations.load() - before_success) << endl;
    cout << "   ❌ Deadlock scenarios: " << (deadlock_scenarios.load() - before_deadlock) << endl;
    cout << "   ⏰ Timeout preventions: " << (timeout_preventions.load() - before_timeout) << endl;
}

int main() {
    cout << "🧪 COMPREHENSIVE DEADLOCK DEMONSTRATION" << endl;
    cout << "=======================================" << endl;
    cout << "This program demonstrates deadlock scenarios and prevention" << endl;
    cout << "techniques in a controlled, non-hanging environment." << endl;
    
    // Test 1: Show deadlock scenario (with timeout safety)
    run_test("Deadlock-Prone Scenario (with timeout safety)", []() {
        cout << "Starting threads that would deadlock without timeouts..." << endl;
        
        thread t1(deadlock_prone_thread_1);
        thread t2(deadlock_prone_thread_2);
        
        t1.join();
        t2.join();
    });
    
    // Give system a moment to reset
    this_thread::sleep_for(milliseconds(100));
    
    // Test 2: Lock ordering prevention
    run_test("Lock Ordering Prevention", []() {
        cout << "Both threads will acquire locks in the same order..." << endl;
        
        thread t1(ordered_thread_1);
        thread t2(ordered_thread_2);
        
        t1.join();
        t2.join();
    });
    
    // Give system a moment to reset
    this_thread::sleep_for(milliseconds(100));
    
    // Test 3: Timeout strategy
    run_test("Timeout-Based Prevention", []() {
        cout << "Using aggressive timeouts to prevent deadlocks..." << endl;
        
        thread t1(timeout_thread_1);
        thread t2(timeout_thread_2);
        
        t1.join();
        t2.join();
    });
    
    // Give system a moment to reset
    this_thread::sleep_for(milliseconds(100));
    
    // Test 4: RAII management
    run_test("RAII Lock Management", []() {
        cout << "Using RAII for automatic lock management..." << endl;
        
        thread t1(raii_thread_example, 1);
        thread t2(raii_thread_example, 2);
        
        t1.join();
        t2.join();
    });
    
    // Test 5: Stress test with multiple threads
    run_test("Multi-Thread Stress Test", []() {
        cout << "Running multiple threads with different strategies..." << endl;
        
        vector<thread> threads;
        threads.emplace_back(ordered_thread_1);
        threads.emplace_back(ordered_thread_2);
        threads.emplace_back(timeout_thread_1);
        threads.emplace_back(timeout_thread_2);
        threads.emplace_back(raii_thread_example, 1);
        threads.emplace_back(raii_thread_example, 2);
        
        for (auto& t : threads) {
            t.join();
        }
    });
    
    cout << "\n" << string(60, '=') << endl;
    cout << "🎉 ALL TESTS COMPLETED SUCCESSFULLY!" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\n📈 FINAL STATISTICS:" << endl;
    cout << "   ✅ Total successful operations: " << successful_operations.load() << endl;
    cout << "   ❌ Total deadlock scenarios detected: " << deadlock_scenarios.load() << endl;
    cout << "   ⏰ Total timeout preventions: " << timeout_preventions.load() << endl;
    
    cout << "\n🎓 KEY LESSONS DEMONSTRATED:" << endl;
    cout << "   1. 🔴 Deadlocks occur when threads acquire locks in different orders" << endl;
    cout << "   2. 🟢 Lock ordering prevents circular wait conditions" << endl;
    cout << "   3. 🟡 Timeouts prevent infinite waiting" << endl;
    cout << "   4. 🔵 RAII ensures automatic resource cleanup" << endl;
    cout << "   5. 📊 Monitoring helps detect and prevent deadlock situations" << endl;
    
    cout << "\n🔧 COMPILATION:" << endl;
    cout << "   g++ -std=c++11 -pthread -O2 -o deadlock_simple DeadLock_Simple.cpp" << endl;
    cout << "   ./deadlock_simple" << endl;
    
    cout << "\n✨ This demonstration shows real deadlock prevention techniques" << endl;
    cout << "   used in production systems without risking infinite hangs!" << endl;
    
    return 0;
}

/*
 * 📚 EDUCATIONAL NOTES:
 * 
 * DEADLOCK FUNDAMENTALS:
 * =====================
 * A deadlock is a circular waiting condition where threads cannot proceed
 * because each is waiting for a resource held by another thread in the cycle.
 * 
 * COFFMAN CONDITIONS (All must be present):
 * 1. Mutual Exclusion: Resources cannot be shared
 * 2. Hold and Wait: Threads hold resources while waiting for others
 * 3. No Preemption: Resources cannot be forcibly taken
 * 4. Circular Wait: Circular dependency in resource allocation
 * 
 * PREVENTION STRATEGIES DEMONSTRATED:
 * ==================================
 * 
 * 1. LOCK ORDERING:
 *    - Establish a global order for lock acquisition
 *    - All threads must acquire locks in the same sequence
 *    - Breaks the circular wait condition
 *    - Example: Always acquire lock A before lock B
 * 
 * 2. TIMEOUT-BASED LOCKING:
 *    - Use try_lock_for() instead of blocking lock()
 *    - Set reasonable timeout values
 *    - Release held locks if timeout occurs
 *    - Retry after backing off
 * 
 * 3. RAII (Resource Acquisition Is Initialization):
 *    - Use smart lock managers and RAII principles
 *    - Automatic resource cleanup on scope exit
 *    - Exception-safe resource management
 *    - Consistent lock acquisition/release patterns
 * 
 * REAL-WORLD APPLICATIONS:
 * =======================
 * - Database transaction management
 * - File system operations
 * - Network protocol implementations
 * - GUI thread synchronization
 * - Memory allocator design
 * - Process scheduling
 * 
 * PERFORMANCE CONSIDERATIONS:
 * ==========================
 * - Lock ordering: Minimal overhead, high effectiveness
 * - Timeouts: Small overhead, good for detection
 * - RAII: No runtime overhead, improves maintainability
 * - Detection algorithms: Higher overhead but allow optimization
 * 
 * DEBUGGING TIPS:
 * ==============
 * - Use thread-safe logging to trace lock acquisitions
 * - Monitor lock hold times and wait times
 * - Implement deadlock detection in debug builds
 * - Use tools like ThreadSanitizer for automatic detection
 * - Design lock hierarchies and document them clearly
 * 
 * This demonstration safely shows deadlock concepts without actually
 * creating infinite hangs, making it suitable for testing and education.
 */
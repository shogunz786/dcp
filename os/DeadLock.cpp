/*
 * PROBLEM: Deadlock Demonstration and Prevention
 *
 * Demonstrate classic deadlock scenarios and various prevention techniques.
 * A deadlock occurs when two or more threads are blocked forever, each
 * waiting for the other to release a resource.
 *
 * DEADLOCK CONDITIONS (Coffman Conditions):
 * 1. Mutual Exclusion: Resources cannot be shared
 * 2. Hold and Wait: Thread holds resources while waiting for others
 * 3. No Preemption: Resources cannot be forcibly taken away
 * 4. Circular Wait: Circular chain of threads waiting for each other
 *
 * PREVENTION STRATEGIES:
 * - Lock Ordering: Always acquire locks in the same order
 * - Timeout-based Locking: Use try_lock with timeouts
 * - Lock Hierarchy: Assign priorities to locks
 * - Deadlock Detection: Monitor for cycles in wait-for graph
 * - Banker's Algorithm: Resource allocation with safety checks
 *
 * EXAMPLES DEMONSTRATED:
 * 1. Classic deadlock scenario
 * 2. Lock ordering prevention
 * 3. Timeout-based prevention
 * 4. RAII-based lock management
 * 5. Deadlock detection simulation
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
#include <condition_variable>
#include <map>
#include <set>
#include <cassert>
#include <random>

using namespace std;
using namespace std::chrono;

// Global statistics for monitoring
atomic<int> deadlock_count(0);
atomic<int> successful_operations(0);
atomic<int> timeout_failures(0);

/**
 * Simple Mutex wrapper for demonstration compatibility
 */
class SimpleMutex
{
private:
	timed_mutex mtx; // Use timed_mutex for try_lock_for support
	string name;

public:
	SimpleMutex(const string &mutex_name) : name(mutex_name) {}

	void wait()
	{
		cout << "[" << this_thread::get_id() << "] Waiting for " << name << endl;
		mtx.lock();
		cout << "[" << this_thread::get_id() << "] Acquired " << name << endl;
	}

	void signal()
	{
		cout << "[" << this_thread::get_id() << "] Releasing " << name << endl;
		mtx.unlock();
	}

	bool try_wait_for(int timeout_ms)
	{
		cout << "[" << this_thread::get_id() << "] Trying " << name
			 << " with timeout " << timeout_ms << "ms" << endl;

		if (mtx.try_lock_for(chrono::milliseconds(timeout_ms)))
		{
			cout << "[" << this_thread::get_id() << "] Acquired " << name
				 << " within timeout" << endl;
			return true;
		}
		else
		{
			cout << "[" << this_thread::get_id() << "] Timeout waiting for "
				 << name << endl;
			return false;
		}
	}

	timed_mutex &get_mutex() { return mtx; }
	const string &get_name() const { return name; }
};

// Global mutexes for deadlock scenarios
SimpleMutex mutex_a("MutexA");
SimpleMutex mutex_b("MutexB");
SimpleMutex mutex_c("MutexC");

/**
 * DEADLOCK SCENARIO 1: Classic Two-Thread Deadlock
 *
 * Thread 1: Acquires A, then tries to acquire B
 * Thread 2: Acquires B, then tries to acquire A
 * Result: Both threads wait forever
 */
void deadlock_thread_1()
{
	cout << "\n=== Deadlock Thread 1 Starting ===" << endl;

	mutex_a.wait();							   // Acquire mutex A
	this_thread::sleep_for(milliseconds(100)); // Give other thread time

	cout << "[Thread1] Now trying to acquire mutex B..." << endl;
	mutex_b.wait(); // Try to acquire mutex B (will deadlock)

	// This code will never execute in deadlock scenario
	cout << "[Thread1] Got both mutexes! Doing work..." << endl;
	this_thread::sleep_for(milliseconds(50));

	mutex_b.signal();
	mutex_a.signal();

	successful_operations++;
	cout << "=== Deadlock Thread 1 Finished ===" << endl;
}

void deadlock_thread_2()
{
	cout << "\n=== Deadlock Thread 2 Starting ===" << endl;

	mutex_b.wait();							   // Acquire mutex B
	this_thread::sleep_for(milliseconds(100)); // Give other thread time

	cout << "[Thread2] Now trying to acquire mutex A..." << endl;
	mutex_a.wait(); // Try to acquire mutex A (will deadlock)

	// This code will never execute in deadlock scenario
	cout << "[Thread2] Got both mutexes! Doing work..." << endl;
	this_thread::sleep_for(milliseconds(50));

	mutex_a.signal();
	mutex_b.signal();

	successful_operations++;
	cout << "=== Deadlock Thread 2 Finished ===" << endl;
}

/**
 * PREVENTION STRATEGY 1: Lock Ordering
 *
 * Always acquire locks in the same order (e.g., A before B)
 * This breaks the circular wait condition
 */
void lock_ordering_thread_1()
{
	cout << "\n=== Lock Ordering Thread 1 Starting ===" << endl;

	// Always acquire A first, then B
	mutex_a.wait();
	cout << "[OrderThread1] Got A, now getting B..." << endl;
	mutex_b.wait();

	cout << "[OrderThread1] Got both mutexes! Doing work..." << endl;
	this_thread::sleep_for(milliseconds(100));

	mutex_b.signal();
	mutex_a.signal();

	successful_operations++;
	cout << "=== Lock Ordering Thread 1 Finished ===" << endl;
}

void lock_ordering_thread_2()
{
	cout << "\n=== Lock Ordering Thread 2 Starting ===" << endl;

	// Same order: A first, then B
	mutex_a.wait();
	cout << "[OrderThread2] Got A, now getting B..." << endl;
	mutex_b.wait();

	cout << "[OrderThread2] Got both mutexes! Doing work..." << endl;
	this_thread::sleep_for(milliseconds(100));

	mutex_b.signal();
	mutex_a.signal();

	successful_operations++;
	cout << "=== Lock Ordering Thread 2 Finished ===" << endl;
}

/**
 * PREVENTION STRATEGY 2: Timeout-based Locking
 *
 * Use timeouts when acquiring locks to avoid infinite waiting
 */
void timeout_thread_1()
{
	cout << "\n=== Timeout Thread 1 Starting ===" << endl;

	mutex_a.wait(); // Get first lock normally

	// Try to get second lock with timeout
	if (mutex_b.try_wait_for(200))
	{ // 200ms timeout
		cout << "[TimeoutThread1] Got both mutexes! Doing work..." << endl;
		this_thread::sleep_for(milliseconds(50));

		mutex_b.signal();
		mutex_a.signal();
		successful_operations++;
	}
	else
	{
		cout << "[TimeoutThread1] Timeout! Releasing first lock..." << endl;
		mutex_a.signal();
		timeout_failures++;
	}

	cout << "=== Timeout Thread 1 Finished ===" << endl;
}

void timeout_thread_2()
{
	cout << "\n=== Timeout Thread 2 Starting ===" << endl;

	mutex_b.wait(); // Get first lock normally

	// Try to get second lock with timeout
	if (mutex_a.try_wait_for(200))
	{ // 200ms timeout
		cout << "[TimeoutThread2] Got both mutexes! Doing work..." << endl;
		this_thread::sleep_for(milliseconds(50));

		mutex_a.signal();
		mutex_b.signal();
		successful_operations++;
	}
	else
	{
		cout << "[TimeoutThread2] Timeout! Releasing first lock..." << endl;
		mutex_b.signal();
		timeout_failures++;
	}

	cout << "=== Timeout Thread 2 Finished ===" << endl;
}

/**
 * PREVENTION STRATEGY 3: RAII Lock Management
 *
 * Use RAII (Resource Acquisition Is Initialization) to automatically
 * manage lock acquisition and release
 */
class RAIILockManager
{
private:
	vector<SimpleMutex *> acquired_locks;

public:
	bool acquire_locks(vector<SimpleMutex *> locks, int timeout_ms = 1000)
	{
		// Sort locks by address to ensure consistent ordering
		sort(locks.begin(), locks.end());

		for (auto *lock : locks)
		{
			if (timeout_ms > 0)
			{
				if (!lock->try_wait_for(timeout_ms / locks.size()))
				{
					// Failed to acquire - release all previously acquired
					release_all();
					return false;
				}
			}
			else
			{
				lock->wait();
			}
			acquired_locks.push_back(lock);
		}
		return true;
	}

	void release_all()
	{
		// Release in reverse order
		for (auto it = acquired_locks.rbegin(); it != acquired_locks.rend(); ++it)
		{
			(*it)->signal();
		}
		acquired_locks.clear();
	}

	~RAIILockManager()
	{
		release_all();
	}
};

void raii_thread_example(int thread_id)
{
	cout << "\n=== RAII Thread " << thread_id << " Starting ===" << endl;

	RAIILockManager lock_manager;
	vector<SimpleMutex *> needed_locks = {&mutex_a, &mutex_b};

	if (lock_manager.acquire_locks(needed_locks, 300))
	{
		cout << "[RAIIThread" << thread_id << "] Got all locks! Doing work..." << endl;
		this_thread::sleep_for(milliseconds(100));
		successful_operations++;
	}
	else
	{
		cout << "[RAIIThread" << thread_id << "] Failed to acquire all locks" << endl;
		timeout_failures++;
	}

	// Locks automatically released by destructor
	cout << "=== RAII Thread " << thread_id << " Finished ===" << endl;
}

/**
 * DEADLOCK DETECTION: Simplified Wait-For Graph
 */
class DeadlockDetector
{
private:
	map<thread::id, set<string>> wait_for_graph;
	mutex graph_mutex;

public:
	void add_wait_edge(thread::id thread, const string &resource)
	{
		lock_guard<mutex> lock(graph_mutex);
		wait_for_graph[thread].insert(resource);
	}

	void remove_wait_edge(thread::id thread, const string &resource)
	{
		lock_guard<mutex> lock(graph_mutex);
		if (wait_for_graph.find(thread) != wait_for_graph.end())
		{
			wait_for_graph[thread].erase(resource);
			if (wait_for_graph[thread].empty())
			{
				wait_for_graph.erase(thread);
			}
		}
	}

	bool detect_cycle()
	{
		lock_guard<mutex> lock(graph_mutex);
		// Simplified cycle detection - in practice would be more complex
		return wait_for_graph.size() > 1; // Multiple threads waiting
	}

	void print_graph()
	{
		lock_guard<mutex> lock(graph_mutex);
		cout << "\n--- Wait-For Graph ---" << endl;
		for (const auto &entry : wait_for_graph)
		{
			cout << "Thread " << entry.first << " waiting for: ";
			for (const auto &resource : entry.second)
			{
				cout << resource << " ";
			}
			cout << endl;
		}
		cout << "---------------------" << endl;
	}
};

DeadlockDetector detector;

/**
 * Run a specific test scenario
 */
void run_test(const string &test_name, function<void()> test_func)
{
	cout << "\n"
		 << string(50, '=') << endl;
	cout << "🧪 " << test_name << endl;
	cout << string(50, '=') << endl;

	// Reset statistics
	successful_operations = 0;
	timeout_failures = 0;

	auto start_time = high_resolution_clock::now();

	try
	{
		test_func();
	}
	catch (const exception &e)
	{
		cout << "❌ Test threw exception: " << e.what() << endl;
	}

	auto end_time = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(end_time - start_time);

	cout << "\n📊 Test Results:" << endl;
	cout << "   Duration: " << duration.count() << "ms" << endl;
	cout << "   Successful operations: " << successful_operations.load() << endl;
	cout << "   Timeout failures: " << timeout_failures.load() << endl;
}

int main()
{
	cout << "🧪 Deadlock Demonstration and Prevention Techniques" << endl;
	cout << "===================================================" << endl;

	// Test 1: Demonstrate Classic Deadlock (with timeout to prevent hanging)
	run_test("Test 1: Classic Deadlock Scenario", []()
			 {
        cout << "Starting two threads that will deadlock..." << endl;
        cout << "⚠️  This test will timeout after 2 seconds to prevent hanging" << endl;
        
        thread t1(deadlock_thread_1);
        thread t2(deadlock_thread_2);
        
        // Give threads time to deadlock, then force termination
        this_thread::sleep_for(seconds(2));
        
        // In a real scenario, these threads would be stuck forever
        // We'll detach them to prevent the test from hanging
        t1.detach();
        t2.detach();
        
        cout << "⚠️  Threads are deadlocked - they would wait forever!" << endl;
        deadlock_count++; });

	// Test 2: Lock Ordering Prevention
	run_test("Test 2: Lock Ordering Prevention", []()
			 {
        cout << "Using consistent lock ordering to prevent deadlock..." << endl;
        
        thread t1(lock_ordering_thread_1);
        thread t2(lock_ordering_thread_2);
        
        t1.join();
        t2.join();
        
        cout << "✅ Both threads completed successfully!" << endl; });

	// Test 3: Timeout-based Prevention
	run_test("Test 3: Timeout-based Prevention", []()
			 {
        cout << "Using timeouts to avoid infinite waiting..." << endl;
        
        thread t1(timeout_thread_1);
        thread t2(timeout_thread_2);
        
        t1.join();
        t2.join();
        
        cout << "✅ Threads completed with timeout handling!" << endl; });

	// Test 4: RAII Lock Management
	run_test("Test 4: RAII Lock Management", []()
			 {
        cout << "Using RAII for automatic lock management..." << endl;
        
        thread t1(raii_thread_example, 1);
        thread t2(raii_thread_example, 2);
        
        t1.join();
        t2.join();
        
        cout << "✅ RAII ensured proper lock cleanup!" << endl; });

	// Test 5: Multiple Threads Stress Test
	run_test("Test 5: Multiple Threads Stress Test", []()
			 {
        cout << "Running multiple threads with various strategies..." << endl;
        
        vector<thread> threads;
        
        // Mix of different strategies
        threads.emplace_back(lock_ordering_thread_1);
        threads.emplace_back(lock_ordering_thread_2);
        threads.emplace_back(timeout_thread_1);
        threads.emplace_back(timeout_thread_2);
        threads.emplace_back(raii_thread_example, 1);
        threads.emplace_back(raii_thread_example, 2);
        
        for (auto& t : threads) {
            t.join();
        }
        
        cout << "✅ All threads in stress test completed!" << endl; });

	cout << "\n"
		 << string(50, '=') << endl;
	cout << "🎉 Deadlock Prevention Tests Completed!" << endl;
	cout << string(50, '=') << endl;

	cout << "\n📋 Summary of Prevention Techniques:" << endl;
	cout << "   1. ✅ Lock Ordering: Always acquire locks in same order" << endl;
	cout << "   2. ✅ Timeouts: Avoid infinite waiting with try_lock_for" << endl;
	cout << "   3. ✅ RAII: Automatic resource management" << endl;
	cout << "   4. ✅ Detection: Monitor wait-for relationships" << endl;

	cout << "\n🔧 To compile and run:" << endl;
	cout << "   g++ -std=c++11 -pthread -o deadlock_test DeadLock.cpp && ./deadlock_test" << endl;

	cout << "\n⚠️  Note: The classic deadlock test intentionally creates a deadlock" << endl;
	cout << "   scenario to demonstrate the problem, then uses detach() to" << endl;
	cout << "   prevent the test program from hanging indefinitely." << endl;

	return 0;
}

/*
 * 📚 DEADLOCK EXPLANATION:
 *
 * WHAT IS A DEADLOCK?
 * A deadlock is a situation where two or more threads are blocked forever,
 * each waiting for the other to release a resource that it needs.
 *
 * COFFMAN CONDITIONS (All must be true for deadlock):
 * 1. Mutual Exclusion: Resources cannot be shared simultaneously
 * 2. Hold and Wait: Thread holds resources while waiting for others
 * 3. No Preemption: Resources cannot be forcibly removed from threads
 * 4. Circular Wait: Circular chain of threads each waiting for the next
 *
 * CLASSIC DEADLOCK SCENARIO:
 * Thread 1: Lock A → (delay) → Lock B → Work → Unlock B → Unlock A
 * Thread 2: Lock B → (delay) → Lock A → Work → Unlock A → Unlock B
 *
 * Result: Thread 1 holds A, waits for B
 *         Thread 2 holds B, waits for A
 *         Neither can proceed = DEADLOCK!
 *
 * PREVENTION STRATEGIES:
 *
 * 1. LOCK ORDERING:
 *    - Always acquire locks in the same global order
 *    - Breaks circular wait condition
 *    - Example: Always A before B, never B before A
 *
 * 2. TIMEOUT-BASED LOCKING:
 *    - Use try_lock_for() with timeout instead of blocking lock()
 *    - If timeout occurs, release held locks and retry
 *    - Prevents infinite waiting
 *
 * 3. RAII (Resource Acquisition Is Initialization):
 *    - Use smart lock managers that automatically release resources
 *    - Exception-safe resource management
 *    - Consistent lock acquisition/release patterns
 *
 * 4. DEADLOCK DETECTION:
 *    - Monitor wait-for relationships between threads
 *    - Detect cycles in the wait-for graph
 *    - Break deadlocks when detected
 *
 * 5. BANKER'S ALGORITHM:
 *    - Pre-allocate maximum resources needed
 *    - Only allow resource allocation if system stays in "safe state"
 *    - Prevents deadlock by avoiding unsafe allocations
 *
 * REAL-WORLD EXAMPLES:
 * - Database transaction deadlocks
 * - File system lock conflicts
 * - Network protocol deadlocks
 * - GUI event handling deadlocks
 * - Memory allocation deadlocks
 *
 * PERFORMANCE IMPACT:
 * - Prevention adds overhead but ensures progress
 * - Detection requires monitoring but allows optimization
 * - Recovery from deadlock can be expensive
 * - Better to prevent than detect and recover
 *
 * ⏱️ COMPLEXITY:
 * - Prevention: O(1) overhead per lock operation
 * - Detection: O(n²) where n = number of threads
 * - Recovery: Variable, depends on rollback strategy
 */
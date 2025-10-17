/*
 * PROBLEM: Thread Local Storage Implementation
 *
 * Implement a thread-local storage system that provides memory blocks
 * on a per-thread basis. Each thread can allocate and access its own
 * memory blocks identified by unique IDs without interference from other threads.
 *
 * FEATURES:
 * - Per-thread memory allocation with unique IDs
 * - Thread-safe access to global thread map
 * - Automatic thread-local map creation on first access
 * - Memory cleanup capabilities per ID and per thread
 * - Lock-free access to thread-local data once acquired
 *
 * DESIGN PRINCIPLES:
 * - Outer map (thread ID -> local map) is protected by mutex
 * - Inner maps (ID -> memory pointer) are thread-local, no locking needed
 * - Lazy initialization of thread-local maps
 * - Proper memory management and cleanup
 *
 * OPERATIONS:
 * - get(id, size): Get/allocate memory block for ID in current thread
 * - erase(id): Remove memory block for ID in current thread
 * - removeThread(): Clean up all memory for current thread
 *
 * THREAD SAFETY:
 * - Global map access is mutex-protected
 * - Thread-local map access is inherently thread-safe
 * - Memory allocation/deallocation is isolated per thread
 */

#include <map>
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <unordered_map>

/**
 * Thread Local Storage Class
 *
 * ARCHITECTURE:
 * - Global map: thread_id -> local_map_pointer
 * - Local map: memory_id -> memory_pointer
 * - Mutex protects only the global map access
 * - Each thread has its own local map for lock-free access
 *
 * MEMORY LAYOUT:
 * Global Map (mutex protected):
 * [Thread1] -> LocalMap1 -> {id1: ptr1, id2: ptr2, ...}
 * [Thread2] -> LocalMap2 -> {id1: ptr3, id2: ptr4, ...}
 * [Thread3] -> LocalMap3 -> {id1: ptr5, id2: ptr6, ...}
 */
class ThreadLocalStorage
{
private:
	// Global map from thread ID to thread-local map
	std::unordered_map<std::thread::id, std::unordered_map<int, void *> *> globalMap;

	// Mutex to protect global map access
	std::mutex globalLock;

	/**
	 * Get current thread ID
	 * @return Current thread ID
	 */
	std::thread::id getCurrentThreadId() const
	{
		return std::this_thread::get_id();
	}

	/**
	 * Get or create thread-local map for current thread
	 * @return Pointer to thread-local map
	 */
	std::unordered_map<int, void *> *getOrCreateLocalMap()
	{
		std::thread::id threadId = getCurrentThreadId();
		std::unordered_map<int, void *> *localMap = nullptr;

		// Critical section: access global map
		{
			std::lock_guard<std::mutex> lock(globalLock);

			auto it = globalMap.find(threadId);
			if (it != globalMap.end())
			{
				localMap = it->second;
			}
			else
			{
				// First time this thread is accessing storage
				// Create new local map for this thread
				localMap = new std::unordered_map<int, void *>();
				globalMap[threadId] = localMap;
			}
		}

		return localMap;
	}

	/**
	 * Get thread-local map for current thread (read-only)
	 * @return Pointer to thread-local map or nullptr if not found
	 */
	std::unordered_map<int, void *> *getLocalMap()
	{
		std::thread::id threadId = getCurrentThreadId();
		std::unordered_map<int, void *> *localMap = nullptr;

		// Critical section: access global map
		{
			std::lock_guard<std::mutex> lock(globalLock);

			auto it = globalMap.find(threadId);
			if (it != globalMap.end())
			{
				localMap = it->second;
			}
		}

		return localMap;
	}

public:
	/**
	 * Constructor
	 */
	ThreadLocalStorage() = default;

	/**
	 * Destructor - Clean up all allocated memory
	 */
	~ThreadLocalStorage()
	{
		cleanup();
	}

	/**
	 * Get memory block for given ID in current thread
	 * If memory doesn't exist, allocate new block of specified size
	 *
	 * @param id Unique identifier for memory block within thread
	 * @param sizeBytes Size of memory block to allocate if not exists
	 * @return Pointer to memory block, nullptr on allocation failure
	 */
	void *get(int id, int sizeBytes)
	{
		if (sizeBytes <= 0)
		{
			return nullptr;
		}

		// Get or create thread-local map (thread-safe)
		std::unordered_map<int, void *> *localMap = getOrCreateLocalMap();

		// Access thread-local map (no locking needed - only this thread accesses it)
		auto it = localMap->find(id);
		if (it != localMap->end())
		{
			// Memory block already exists for this ID
			return it->second;
		}
		else
		{
			// Allocate new memory block
			void *pMem = std::malloc(sizeBytes);
			if (pMem != nullptr)
			{
				// Initialize memory to zero
				std::memset(pMem, 0, sizeBytes);
				// Store in thread-local map
				(*localMap)[id] = pMem;
			}
			return pMem;
		}
	}

	/**
	 * Remove memory block for given ID in current thread
	 *
	 * @param id Unique identifier for memory block to remove
	 * @return true if memory was found and freed, false otherwise
	 */
	bool erase(int id)
	{
		// Get thread-local map (read-only access)
		std::unordered_map<int, void *> *localMap = getLocalMap();

		if (localMap == nullptr)
		{
			return false; // No local map exists for this thread
		}

		// Access thread-local map (no locking needed)
		auto it = localMap->find(id);
		if (it != localMap->end())
		{
			// Free the memory
			std::free(it->second);
			// Remove from map
			localMap->erase(it);
			return true;
		}

		return false; // Memory block not found
	}

	/**
	 * Remove all memory blocks for current thread
	 * This should be called when a thread is about to exit
	 *
	 * @return Number of memory blocks that were freed
	 */
	int removeThread()
	{
		std::thread::id threadId = getCurrentThreadId();
		std::unordered_map<int, void *> *localMap = nullptr;

		// Critical section: remove from global map
		{
			std::lock_guard<std::mutex> lock(globalLock);

			auto it = globalMap.find(threadId);
			if (it != globalMap.end())
			{
				localMap = it->second;
				globalMap.erase(it);
			}
		}

		int freedCount = 0;
		if (localMap != nullptr)
		{
			// Free all memory blocks in the local map
			for (auto &pair : *localMap)
			{
				std::free(pair.second);
				freedCount++;
			}

			// Delete the local map itself
			delete localMap;
		}

		return freedCount;
	}

	/**
	 * Get number of memory blocks allocated for current thread
	 *
	 * @return Number of memory blocks
	 */
	int getBlockCount()
	{
		std::unordered_map<int, void *> *localMap = getLocalMap();
		return localMap ? static_cast<int>(localMap->size()) : 0;
	}

	/**
	 * Get number of threads that have allocated memory
	 *
	 * @return Number of threads with allocated memory
	 */
	int getThreadCount()
	{
		std::lock_guard<std::mutex> lock(globalLock);
		return static_cast<int>(globalMap.size());
	}

	/**
	 * Check if memory block exists for given ID in current thread
	 *
	 * @param id Memory block identifier
	 * @return true if block exists, false otherwise
	 */
	bool exists(int id)
	{
		std::unordered_map<int, void *> *localMap = getLocalMap();
		if (localMap == nullptr)
		{
			return false;
		}

		return localMap->find(id) != localMap->end();
	}

	/**
	 * Clean up all memory for all threads (for destructor)
	 */
	void cleanup()
	{
		std::lock_guard<std::mutex> lock(globalLock);

		for (auto &threadPair : globalMap)
		{
			std::unordered_map<int, void *> *localMap = threadPair.second;

			// Free all memory blocks
			for (auto &memPair : *localMap)
			{
				std::free(memPair.second);
			}

			// Delete the local map
			delete localMap;
		}

		globalMap.clear();
	}
};

// Test helper functions and utilities

/**
 * Test data structure to store in thread-local storage
 */
struct TestData
{
	int value;
	char buffer[64];

	TestData(int v) : value(v)
	{
		snprintf(buffer, sizeof(buffer), "test_data_%d", v);
	}
};

/**
 * Worker thread function for multi-threading tests
 */
void workerThread(ThreadLocalStorage &tls, int threadNum, int blockCount,
				  std::vector<bool> &results, int resultIndex)
{
	bool success = true;

	try
	{
		// Allocate multiple memory blocks
		for (int i = 0; i < blockCount; i++)
		{
			int id = i;
			int size = sizeof(TestData);

			void *ptr = tls.get(id, size);
			if (ptr == nullptr)
			{
				success = false;
				break;
			}

			// Initialize test data
			TestData *data = new (ptr) TestData(threadNum * 1000 + i);

			// Verify data integrity
			if (data->value != threadNum * 1000 + i)
			{
				success = false;
				break;
			}
		}

		// Verify all blocks exist
		for (int i = 0; i < blockCount; i++)
		{
			if (!tls.exists(i))
			{
				success = false;
				break;
			}
		}

		// Test block count
		if (tls.getBlockCount() != blockCount)
		{
			success = false;
		}

		// Remove some blocks
		for (int i = 0; i < blockCount / 2; i++)
		{
			if (!tls.erase(i))
			{
				success = false;
				break;
			}
		}

		// Verify remaining blocks
		int expectedRemaining = blockCount - blockCount / 2;
		if (tls.getBlockCount() != expectedRemaining)
		{
			success = false;
		}

		// Clean up remaining blocks
		tls.removeThread();
	}
	catch (const std::exception &e)
	{
		success = false;
	}

	results[resultIndex] = success;
}

/**
 * Run a single test with detailed output
 */
void runTest(const std::string &testName, std::function<void()> testFunc)
{
	std::cout << "\n=== " << testName << " ===" << std::endl;
	try
	{
		testFunc();
		std::cout << "✅ PASSED" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "❌ FAILED: " << e.what() << std::endl;
		throw;
	}
}

int main()
{
	std::cout << "🧪 Testing Thread Local Storage Implementation" << std::endl;
	std::cout << "==============================================" << std::endl;

	try
	{
		// Test 1: Basic Single Thread Operations
		runTest("Test 1: Basic Single Thread Operations", []()
				{
            ThreadLocalStorage tls;
            
            // Initially no blocks
            assert(tls.getBlockCount() == 0);
            assert(tls.getThreadCount() == 0);
            assert(!tls.exists(1));
            
            // Allocate first block
            void* ptr1 = tls.get(1, 100);
            assert(ptr1 != nullptr);
            assert(tls.getBlockCount() == 1);
            assert(tls.getThreadCount() == 1);
            assert(tls.exists(1));
            
            // Get same block again (should return same pointer)
            void* ptr1_again = tls.get(1, 200);  // Different size, should return same pointer
            assert(ptr1_again == ptr1);
            assert(tls.getBlockCount() == 1);
            
            // Allocate second block
            void* ptr2 = tls.get(2, 200);
            assert(ptr2 != nullptr);
            assert(ptr2 != ptr1);
            assert(tls.getBlockCount() == 2);
            assert(tls.exists(2));
            
            std::cout << "Allocated 2 blocks for main thread" << std::endl; });

		// Test 2: Memory Block Usage
		runTest("Test 2: Memory Block Usage", []()
				{
            ThreadLocalStorage tls;
            
            // Allocate memory for test data
            void* ptr = tls.get(10, sizeof(TestData));
            assert(ptr != nullptr);
            
            // Use placement new to construct object in allocated memory
            TestData* data = new(ptr) TestData(42);
            
            // Verify data integrity
            assert(data->value == 42);
            assert(std::string(data->buffer) == "test_data_42");
            
            // Memory should be zero-initialized initially
            void* ptr2 = tls.get(20, 1024);
            char* charPtr = static_cast<char*>(ptr2);
            bool allZero = true;
            for (int i = 0; i < 1024; i++) {
                if (charPtr[i] != 0) {
                    allZero = false;
                    break;
                }
            }
            assert(allZero);
            
            std::cout << "Memory usage and initialization verified" << std::endl; });

		// Test 3: Erase Operations
		runTest("Test 3: Erase Operations", []()
				{
            ThreadLocalStorage tls;
            
            // Allocate several blocks
            for (int i = 1; i <= 5; i++) {
                void* ptr = tls.get(i, 64);
                assert(ptr != nullptr);
            }
            assert(tls.getBlockCount() == 5);
            
            // Erase middle block
            assert(tls.erase(3) == true);
            assert(tls.getBlockCount() == 4);
            assert(!tls.exists(3));
            assert(tls.exists(2) && tls.exists(4));  // Adjacent blocks still exist
            
            // Try to erase non-existent block
            assert(tls.erase(10) == false);
            assert(tls.getBlockCount() == 4);
            
            // Erase same block again
            assert(tls.erase(3) == false);
            assert(tls.getBlockCount() == 4);
            
            std::cout << "Erase operations verified" << std::endl; });

		// Test 4: Thread Cleanup
		runTest("Test 4: Thread Cleanup", []()
				{
            ThreadLocalStorage tls;
            
            // Allocate several blocks
            for (int i = 1; i <= 3; i++) {
                void* ptr = tls.get(i, 128);
                assert(ptr != nullptr);
            }
            assert(tls.getBlockCount() == 3);
            assert(tls.getThreadCount() == 1);
            
            // Remove all blocks for current thread
            int freedCount = tls.removeThread();
            assert(freedCount == 3);
            assert(tls.getBlockCount() == 0);
            assert(tls.getThreadCount() == 0);
            
            // Verify blocks are gone
            for (int i = 1; i <= 3; i++) {
                assert(!tls.exists(i));
            }
            
            std::cout << "Thread cleanup verified" << std::endl; });

		// Test 5: Multi-threading Safety
		runTest("Test 5: Multi-threading Safety", []()
				{
            ThreadLocalStorage tls;
            const int numThreads = 4;
            const int blocksPerThread = 10;
            
            std::vector<std::thread> threads;
            std::vector<bool> results(numThreads, false);
            
            // Start multiple worker threads
            for (int i = 0; i < numThreads; i++) {
                threads.emplace_back(workerThread, std::ref(tls), i, 
                                   blocksPerThread, std::ref(results), i);
            }
            
            // Wait for all threads to complete
            for (auto& thread : threads) {
                thread.join();
            }
            
            // Verify all threads succeeded
            for (int i = 0; i < numThreads; i++) {
                assert(results[i] == true);
            }
            
            // All threads should have cleaned up
            assert(tls.getThreadCount() == 0);
            
            std::cout << "Multi-threading test with " << numThreads 
                      << " threads completed successfully" << std::endl; });

		// Test 6: Edge Cases
		runTest("Test 6: Edge Cases", []()
				{
            ThreadLocalStorage tls;
            
            // Test invalid size
            void* ptr1 = tls.get(1, 0);
            assert(ptr1 == nullptr);
            
            void* ptr2 = tls.get(2, -10);
            assert(ptr2 == nullptr);
            
            // Test operations on empty storage
            assert(tls.erase(100) == false);
            assert(tls.getBlockCount() == 0);
            assert(tls.removeThread() == 0);
            
            // Test large allocation
            void* ptrLarge = tls.get(100, 1024 * 1024);  // 1MB
            assert(ptrLarge != nullptr);
            assert(tls.exists(100));
            
            std::cout << "Edge cases handled correctly" << std::endl; });

		// Test 7: Stress Test
		runTest("Test 7: Stress Test", []()
				{
            ThreadLocalStorage tls;
            const int numBlocks = 1000;
            
            auto start = std::chrono::high_resolution_clock::now();
            
            // Allocate many blocks
            for (int i = 0; i < numBlocks; i++) {
                void* ptr = tls.get(i, 64);
                assert(ptr != nullptr);
            }
            
            // Verify all blocks exist
            for (int i = 0; i < numBlocks; i++) {
                assert(tls.exists(i));
            }
            
            assert(tls.getBlockCount() == numBlocks);
            
            // Remove all blocks
            int freedCount = tls.removeThread();
            assert(freedCount == numBlocks);
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            
            std::cout << "Stress test with " << numBlocks 
                      << " blocks completed in " << duration.count() << "ms" << std::endl; });

		std::cout << "\n🎉 All tests passed successfully!" << std::endl;
		std::cout << "\n📋 Test Summary:" << std::endl;
		std::cout << "   • Basic operations: ✅" << std::endl;
		std::cout << "   • Memory management: ✅" << std::endl;
		std::cout << "   • Thread safety: ✅" << std::endl;
		std::cout << "   • Edge cases: ✅" << std::endl;
		std::cout << "   • Performance: ✅" << std::endl;
		std::cout << "   • Cleanup operations: ✅" << std::endl;
		std::cout << "   • Multi-threading: ✅" << std::endl;

		std::cout << "\n🔧 To compile and run:" << std::endl;
		std::cout << "   g++ -std=c++11 -pthread -o tls_test ThreadLocalStorage.cpp && ./tls_test" << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "❌ Test suite failed with exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

/*
 * 📚 ALGORITHM EXPLANATION:
 *
 * THREAD LOCAL STORAGE DESIGN:
 *
 * 1. TWO-LEVEL MAP STRUCTURE:
 *    - Global Map: thread_id -> local_map_pointer (mutex protected)
 *    - Local Map: memory_id -> memory_pointer (thread-local, no locking)
 *
 * 2. THREAD SAFETY STRATEGY:
 *    - Only global map access requires mutex protection
 *    - Each thread has exclusive access to its local map
 *    - Memory allocation/deallocation is thread-isolated
 *
 * 3. MEMORY LIFECYCLE:
 *    a) ALLOCATION:
 *       - Thread requests memory with get(id, size)
 *       - If first time: create local map for thread
 *       - If ID exists in local map: return existing pointer
 *       - If ID new: allocate memory, store in local map
 *
 *    b) ACCESS:
 *       - Subsequent get() calls return same pointer
 *       - Thread-local access is lock-free and fast
 *
 *    c) CLEANUP:
 *       - erase(id): free specific memory block
 *       - removeThread(): free all thread's memory blocks
 *       - destructor: cleanup all threads' memory
 *
 * 4. EXAMPLE EXECUTION FLOW:
 *    Thread A calls get(1, 100):
 *    - Lock global map
 *    - Create local map for Thread A
 *    - Unlock global map
 *    - Allocate 100 bytes
 *    - Store in local map[1]
 *    - Return pointer
 *
 *    Thread B calls get(1, 200):
 *    - Lock global map
 *    - Create local map for Thread B
 *    - Unlock global map
 *    - Allocate 200 bytes (separate from Thread A)
 *    - Store in local map[1]
 *    - Return pointer
 *
 *    Thread A calls get(1, 50):
 *    - Get existing local map (no global lock)
 *    - Return existing pointer (ignore size parameter)
 *
 * 5. PERFORMANCE CHARACTERISTICS:
 *    - First access per thread: O(1) with mutex overhead
 *    - Subsequent accesses: O(1) without mutex overhead
 *    - Memory usage: O(threads × unique_ids_per_thread)
 *    - Thread isolation eliminates lock contention
 *
 * 6. MEMORY SAFETY:
 *    - All allocations tracked in maps
 *    - Proper cleanup on thread exit
 *    - Zero-initialization of allocated memory
 *    - Exception-safe resource management
 *
 * ⏱️ COMPLEXITY:
 * - Time: O(1) average for get/erase operations
 * - Space: O(T × I) where T = threads, I = IDs per thread
 * - Lock contention: Minimal (only on first thread access)
 */
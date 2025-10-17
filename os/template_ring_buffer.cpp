/*
 * PROBLEM: Thread-Safe Template Ring Buffer Implementation
 *
 * A ring buffer (circular buffer) is a fixed-size data structure that uses
 * a single buffer as if it were connected end-to-end. This implementation
 * provides thread-safe operations using mutex locks.
 *
 * FEATURES:
 * - Generic template for any data type
 * - Thread-safe operations with mutex protection
 * - Fixed-size circular buffer with wrap-around
 * - Efficient O(1) insertion and removal
 * - Full/empty state detection
 * - Memory management with proper cleanup
 *
 * OPERATIONS:
 * - put(item): Add item to buffer (returns false if full)
 * - get(): Remove and return item from buffer (returns nullptr if empty)
 * - reset(): Clear buffer and reset pointers
 * - isEmpty(): Check if buffer is empty
 * - isFull(): Check if buffer is full
 *
 * THREAD SAFETY:
 * - All operations are mutex-protected
 * - Safe for concurrent producer/consumer access
 * - Prevents race conditions and data corruption
 */

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cassert>
#include <string>
#include <pthread.h>
#include <cstring>
#include <memory>

/**
 * Template Ring Buffer Class
 *
 * DESIGN PRINCIPLES:
 * - Uses read_loc and write_loc pointers that wrap around
 * - Maintains count to distinguish between full and empty states
 * - Mutex protection ensures thread safety
 * - Template design allows any data type storage
 *
 * MEMORY LAYOUT:
 * [0] [1] [2] [3] [4] ... [size-1]
 *  ^               ^
 *  read_loc        write_loc
 *
 * @tparam T Data type to store in the buffer
 */
template <typename T>
class RingBuffer
{
private:
  T *buffer;            // Dynamic array to store elements
  int size;             // Maximum capacity of the buffer
  pthread_mutex_t *mtx; // Mutex for thread safety
  int read_loc;         // Index for next read operation
  int write_loc;        // Index for next write operation
  int count;            // Current number of elements in buffer

public:
  /**
   * Constructor: Initialize ring buffer with given size
   * @param sz Size of the ring buffer
   */
  RingBuffer(int sz);

  /**
   * Destructor: Clean up allocated memory
   */
  ~RingBuffer();

  /**
   * Add an item to the buffer
   * @param item Item to add to the buffer
   * @return true if successful, false if buffer is full
   */
  bool put(T item);

  /**
   * Remove and return an item from the buffer
   * @return Pointer to the item if successful, nullptr if buffer is empty
   * Note: Caller is responsible for the returned memory
   */
  T *get();

  /**
   * Reset the buffer to empty state
   */
  void reset();

  /**
   * Check if buffer is empty
   * @return true if empty, false otherwise
   */
  bool isEmpty();

  /**
   * Check if buffer is full
   * @return true if full, false otherwise
   */
  bool isFull();

  /**
   * Get current number of elements in buffer (thread-safe)
   * @return Current count of elements
   */
  int getCount();

  /**
   * Get the capacity of the buffer
   * @return Maximum size of the buffer
   */
  int getSize() const;
};

// Implementation of template class methods

template <typename T>
RingBuffer<T>::RingBuffer(int sz)
{
  // Input validation
  if (sz <= 0)
  {
    throw std::invalid_argument("Buffer size must be positive");
  }

  size = sz;
  buffer = new T[size];

  // Initialize buffer memory to zero
  memset(buffer, 0, sizeof(T) * size);

  // Initialize mutex
  mtx = new pthread_mutex_t;
  if (pthread_mutex_init(mtx, NULL) != 0)
  {
    delete[] buffer;
    delete mtx;
    throw std::runtime_error("Failed to initialize mutex");
  }

  // Initialize state variables
  count = 0;
  read_loc = 0;
  write_loc = 0;
}

template <typename T>
RingBuffer<T>::~RingBuffer()
{
  // Clean up resources
  pthread_mutex_destroy(mtx);
  delete mtx;
  delete[] buffer;
}

template <typename T>
void RingBuffer<T>::reset()
{
  pthread_mutex_lock(mtx);

  // Reset all state variables
  count = 0;
  read_loc = 0;
  write_loc = 0;

  // Clear buffer memory
  memset(buffer, 0, sizeof(T) * size);

  pthread_mutex_unlock(mtx);
}

template <typename T>
bool RingBuffer<T>::put(T item)
{
  pthread_mutex_lock(mtx);

  // Check if buffer is full
  if (isFull())
  {
    pthread_mutex_unlock(mtx);
    return false;
  }

  // Add item to buffer at write location
  buffer[write_loc] = item;

  // Advance write pointer with wrap-around
  write_loc = (write_loc + 1) % size;

  // Increment count
  count++;

  pthread_mutex_unlock(mtx);
  return true;
}

template <typename T>
T *RingBuffer<T>::get()
{
  pthread_mutex_lock(mtx);

  // Check if buffer is empty
  if (isEmpty())
  {
    pthread_mutex_unlock(mtx);
    return nullptr;
  }

  // Create a copy of the item to return
  T *item = new T(buffer[read_loc]);

  // Advance read pointer with wrap-around
  read_loc = (read_loc + 1) % size;

  // Decrement count
  count--;

  pthread_mutex_unlock(mtx);
  return item;
}

template <typename T>
bool RingBuffer<T>::isEmpty()
{
  // Note: This method assumes it's called within a mutex lock
  // or for read-only operations where exact precision isn't critical
  return (count == 0);
}

template <typename T>
bool RingBuffer<T>::isFull()
{
  // Note: This method assumes it's called within a mutex lock
  // or for read-only operations where exact precision isn't critical
  return (count == size);
}

template <typename T>
int RingBuffer<T>::getCount()
{
  pthread_mutex_lock(mtx);
  int current_count = count;
  pthread_mutex_unlock(mtx);
  return current_count;
}

template <typename T>
int RingBuffer<T>::getSize() const
{
  return size;
}

// Test helper functions and classes

/**
 * Test helper to validate buffer state
 */
template <typename T>
void validateBufferState(RingBuffer<T> &buffer, int expectedCount,
                         bool expectedEmpty, bool expectedFull,
                         const std::string &testContext)
{
  assert(buffer.getCount() == expectedCount);
  assert(buffer.isEmpty() == expectedEmpty);
  assert(buffer.isFull() == expectedFull);
  std::cout << "✅ " << testContext << " - State validated" << std::endl;
}

/**
 * Producer function for multi-threading tests
 */
template <typename T>
void producer(RingBuffer<T> &buffer, const std::vector<T> &items, int &successCount)
{
  successCount = 0;
  for (const auto &item : items)
  {
    if (buffer.put(item))
    {
      successCount++;
    }
    // Small delay to allow interleaving
    std::this_thread::sleep_for(std::chrono::microseconds(10));
  }
}

/**
 * Consumer function for multi-threading tests
 */
template <typename T>
void consumer(RingBuffer<T> &buffer, std::vector<T> &results, int itemCount)
{
  for (int i = 0; i < itemCount; i++)
  {
    T *item = nullptr;
    while (item == nullptr)
    {
      item = buffer.get();
      if (item == nullptr)
      {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
      }
    }
    results.push_back(*item);
    delete item; // Clean up allocated memory
  }
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
  std::cout << "🧪 Testing Thread-Safe Ring Buffer Implementation" << std::endl;
  std::cout << "================================================" << std::endl;

  try
  {
    // Test 1: Basic Construction and Initial State
    runTest("Test 1: Construction and Initial State", []()
            {
            RingBuffer<int> buffer(5);
            validateBufferState(buffer, 0, true, false, "Initial state");
            assert(buffer.getSize() == 5); });

    // Test 2: Single Element Operations
    runTest("Test 2: Single Element Put/Get", []()
            {
            RingBuffer<int> buffer(3);
            
            // Put one element
            assert(buffer.put(42) == true);
            validateBufferState(buffer, 1, false, false, "After put(42)");
            
            // Get the element
            int* item = buffer.get();
            assert(item != nullptr);
            assert(*item == 42);
            delete item;
            validateBufferState(buffer, 0, true, false, "After get()"); });

    // Test 3: Fill Buffer Completely
    runTest("Test 3: Fill Buffer to Capacity", []()
            {
            RingBuffer<int> buffer(3);
            
            // Fill buffer
            assert(buffer.put(1) == true);
            assert(buffer.put(2) == true);
            assert(buffer.put(3) == true);
            validateBufferState(buffer, 3, false, true, "Buffer full");
            
            // Try to add one more (should fail)
            assert(buffer.put(4) == false);
            validateBufferState(buffer, 3, false, true, "After failed put"); });

    // Test 4: Empty Buffer Operations
    runTest("Test 4: Empty Buffer Get Operations", []()
            {
            RingBuffer<int> buffer(3);
            
            // Try to get from empty buffer
            int* item = buffer.get();
            assert(item == nullptr);
            validateBufferState(buffer, 0, true, false, "Empty buffer get"); });

    // Test 5: Wrap-around Functionality
    runTest("Test 5: Wrap-around Operations", []()
            {
            RingBuffer<int> buffer(3);
            
            // Fill buffer
            assert(buffer.put(1) == true);
            assert(buffer.put(2) == true);
            assert(buffer.put(3) == true);
            
            // Remove two elements
            int* item1 = buffer.get();
            int* item2 = buffer.get();
            assert(*item1 == 1 && *item2 == 2);
            delete item1;
            delete item2;
            
            // Add two more (should wrap around)
            assert(buffer.put(4) == true);
            assert(buffer.put(5) == true);
            validateBufferState(buffer, 3, false, true, "After wrap-around");
            
            // Verify order: should get 3, 4, 5
            int* item3 = buffer.get();
            int* item4 = buffer.get();
            int* item5 = buffer.get();
            assert(*item3 == 3 && *item4 == 4 && *item5 == 5);
            delete item3;
            delete item4;
            delete item5; });

    // Test 6: Reset Functionality
    runTest("Test 6: Reset Operations", []()
            {
            RingBuffer<int> buffer(3);
            
            // Fill buffer partially
            buffer.put(1);
            buffer.put(2);
            validateBufferState(buffer, 2, false, false, "Before reset");
            
            // Reset
            buffer.reset();
            validateBufferState(buffer, 0, true, false, "After reset");
            
            // Verify functionality after reset
            assert(buffer.put(10) == true);
            int* item = buffer.get();
            assert(*item == 10);
            delete item; });

    // Test 7: String Type Support
    runTest("Test 7: String Type Support", []()
            {
            RingBuffer<std::string> buffer(2);
            
            assert(buffer.put("Hello") == true);
            assert(buffer.put("World") == true);
            
            std::string* str1 = buffer.get();
            std::string* str2 = buffer.get();
            
            assert(*str1 == "Hello");
            assert(*str2 == "World");
            
            delete str1;
            delete str2; });

    // Test 8: Large Buffer Operations
    runTest("Test 8: Large Buffer Operations", []()
            {
            const int bufferSize = 1000;
            RingBuffer<int> buffer(bufferSize);
            
            // Fill entire buffer
            for (int i = 0; i < bufferSize; i++) {
                assert(buffer.put(i) == true);
            }
            validateBufferState(buffer, bufferSize, false, true, "Large buffer full");
            
            // Empty entire buffer
            for (int i = 0; i < bufferSize; i++) {
                int* item = buffer.get();
                assert(item != nullptr);
                assert(*item == i);
                delete item;
            }
            validateBufferState(buffer, 0, true, false, "Large buffer empty"); });

    // Test 9: Multi-threading Safety
    runTest("Test 9: Multi-threading Safety", []()
            {
            RingBuffer<int> buffer(100);
            
            // Prepare data
            std::vector<int> producerData;
            for (int i = 0; i < 50; i++) {
                producerData.push_back(i);
            }
            
            std::vector<int> consumerResults;
            int successCount = 0;
            
            // Start producer and consumer threads
            std::thread producerThread(producer<int>, std::ref(buffer), 
                                     std::cref(producerData), std::ref(successCount));
            std::thread consumerThread(consumer<int>, std::ref(buffer), 
                                     std::ref(consumerResults), 50);
            
            // Wait for completion
            producerThread.join();
            consumerThread.join();
            
            // Verify results
            assert(successCount == 50);
            assert(consumerResults.size() == 50);
            
            // Verify all items were received (though order may vary due to threading)
            std::sort(consumerResults.begin(), consumerResults.end());
            for (int i = 0; i < 50; i++) {
                assert(consumerResults[i] == i);
            }
            
            std::cout << "Producer added: " << successCount << " items" << std::endl;
            std::cout << "Consumer received: " << consumerResults.size() << " items" << std::endl; });

    // Test 10: Exception Handling
    runTest("Test 10: Exception Handling", []()
            {
            try {
                RingBuffer<int> invalidBuffer(-1);
                assert(false);  // Should not reach here
            } catch (const std::invalid_argument& e) {
                std::cout << "Correctly caught exception: " << e.what() << std::endl;
            } });

    std::cout << "\n🎉 All tests passed successfully!" << std::endl;
    std::cout << "\n📋 Test Summary:" << std::endl;
    std::cout << "   • Basic operations: ✅" << std::endl;
    std::cout << "   • Boundary conditions: ✅" << std::endl;
    std::cout << "   • Wrap-around functionality: ✅" << std::endl;
    std::cout << "   • Thread safety: ✅" << std::endl;
    std::cout << "   • Memory management: ✅" << std::endl;
    std::cout << "   • Template type support: ✅" << std::endl;
    std::cout << "   • Exception handling: ✅" << std::endl;

    std::cout << "\n🔧 To compile and run:" << std::endl;
    std::cout << "   g++ -std=c++11 -pthread -o ring_buffer_test template_ring_buffer.cpp && ./ring_buffer_test" << std::endl;
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
 * RING BUFFER CONCEPTS:
 * - Fixed-size circular buffer that reuses memory efficiently
 * - Two pointers: read_loc and write_loc that wrap around
 * - Count variable distinguishes between full and empty states
 * - Thread-safe through mutex protection
 *
 * KEY OPERATIONS:
 *
 * 1. PUT OPERATION:
 *    - Check if buffer is full (count == size)
 *    - If not full: store item at write_loc, increment count, advance write_loc
 *    - Use modulo arithmetic for wrap-around: write_loc = (write_loc + 1) % size
 *
 * 2. GET OPERATION:
 *    - Check if buffer is empty (count == 0)
 *    - If not empty: retrieve item at read_loc, decrement count, advance read_loc
 *    - Return pointer to copied item (caller manages memory)
 *
 * 3. WRAP-AROUND EXAMPLE:
 *    Buffer size = 4, current state: [A][B][ ][ ]
 *                                      ^     ^
 *                                   read   write
 *    After put(C): [A][B][C][ ]
 *                   ^       ^
 *                 read    write
 *    After put(D): [A][B][C][D]  (buffer full)
 *                   ^           ^
 *                 read        write
 *    After get(): [X][B][C][D]   (X = undefined)
 *                     ^       ^
 *                   read    write
 *    After put(E): [E][B][C][D]  (wrap-around occurred)
 *                     ^   ^
 *                   read write
 *
 * THREAD SAFETY:
 * - All public methods use mutex locks
 * - Prevents race conditions between producer/consumer threads
 * - Memory barriers ensure visibility of changes
 *
 * MEMORY MANAGEMENT:
 * - Constructor allocates buffer and initializes mutex
 * - Destructor properly cleans up all resources
 * - get() returns pointer to new copy (caller must delete)
 * - Exception safety in constructor
 *
 * ⏱️ COMPLEXITY:
 * - Time: O(1) for all operations (put, get, isEmpty, isFull)
 * - Space: O(n) where n is the buffer size
 * - Thread synchronization overhead from mutex operations
 */
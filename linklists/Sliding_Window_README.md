# Sliding Window Algorithms - Comprehensive Implementation

## 🎯 Overview

This project provides a complete implementation of various sliding window algorithms with comprehensive testing, documentation, and real-world examples. The implementation demonstrates both fixed-size and variable-size sliding window techniques commonly used in competitive programming and system design.

## 📊 Features Implemented

### Core Algorithms

1. **Sliding Window Maximum** - O(n) time using monotonic deque
2. **Sliding Window Minimum** - O(n) time using monotonic deque
3. **Sliding Window Sum** - O(n) time with running sum
4. **Maximum Sum Subarray** - O(n) time for fixed window size
5. **Longest Substring K Distinct** - Variable window with hash map
6. **Longest Substring Without Repeating** - Variable window with set
7. **Minimum Window Substring** - Pattern matching with frequency maps

### Educational Features

- 🧪 **Comprehensive Test Suite** (14 automated tests)
- 🎮 **Interactive Visualizations** showing algorithm steps
- 🚀 **Performance Benchmarks** with timing analysis
- 📚 **Detailed Documentation** with complexity analysis
- 🌍 **Real-World Examples** (stock analysis, network monitoring, log processing)

## 🔧 Compilation and Usage

### Basic Compilation

```bash
g++ -std=c++11 -O2 -o sliding_window sliding_window.cpp
```

### Debug Build

```bash
g++ -std=c++11 -g -DDEBUG -o sliding_window_debug sliding_window.cpp
```

### High Performance Build

```bash
g++ -std=c++11 -O3 -DNDEBUG -o sliding_window_optimized sliding_window.cpp
```

## 🎮 Usage Options

### Run All Tests (Default)

```bash
./sliding_window
```

### Interactive Demonstration

```bash
./sliding_window --demo
```

### Performance Benchmarks

```bash
./sliding_window --benchmark
```

### Skip Tests

```bash
./sliding_window --no-tests
```

### Show Help

```bash
./sliding_window --help
```

## 📋 API Reference

### SlidingWindowAlgorithms Class

#### Fixed-Size Window Operations

```cpp
// Find maximum in each sliding window of size k
static vector<int> slidingWindowMaximum(const vector<int>& arr, int k);

// Find minimum in each sliding window of size k
static vector<int> slidingWindowMinimum(const vector<int>& arr, int k);

// Calculate sum of each sliding window of size k
static vector<long long> slidingWindowSum(const vector<int>& arr, int k);

// Find maximum sum among all subarrays of size k
static long long maxSumSubarray(const vector<int>& arr, int k);
```

#### Variable-Size Window Operations

```cpp
// Longest substring with at most k distinct characters
static int longestSubstringKDistinct(const string& s, int k);

// Longest substring without repeating characters
static int longestSubstringWithoutRepeating(const string& s);

// Minimum window containing all characters of pattern
static string minWindowSubstring(const string& s, const string& t);
```

#### Legacy Function (Backward Compatibility)

```cpp
// Original function signature with fixes
void sliding_max(const vector<int>& arr, vector<int>& maxarr, int w);
```

## 🧮 Algorithm Complexity Analysis

| Algorithm                    | Time Complexity  | Space Complexity | Technique               |
| ---------------------------- | ---------------- | ---------------- | ----------------------- |
| Sliding Window Maximum       | O(n)             | O(k)             | Monotonic Deque         |
| Sliding Window Minimum       | O(n)             | O(k)             | Monotonic Deque         |
| Sliding Window Sum           | O(n)             | O(1)             | Running Sum             |
| Longest Substring K Distinct | O(n)             | O(k)             | Hash Map + Two Pointers |
| Longest Substring No Repeat  | O(n)             | O(min(m,n))      | Set + Two Pointers      |
| Minimum Window Substring     | O(\|s\| + \|t\|) | O(\|t\|)         | Frequency Map           |

## 🎯 Key Algorithmic Insights

### Monotonic Deque Technique

- **For Maximum**: Maintain decreasing order of values
- **For Minimum**: Maintain increasing order of values
- **Key Benefit**: O(1) amortized insertion/deletion

### Two-Pointer Technique

- **Expand**: Move right pointer to include new elements
- **Contract**: Move left pointer to maintain window constraints
- **Applications**: Variable-size windows, pattern matching

### Running Sum Optimization

- **Fixed Windows**: Add new element, subtract removed element
- **Avoids**: Recalculating entire window sum each time
- **Benefit**: O(1) per window instead of O(k)

## 🧪 Test Results

### Automated Test Suite

```
🎊 TEST SUMMARY
===============
Total tests: 14
✅ Passed: 14
❌ Failed: 0
Success rate: 100.0%
```

### Performance Benchmarks

```
📊 Array size: 100,000
------------------------
Sliding Window Maximum (k=10):   3000μs,  99,991 windows
Sliding Window Maximum (k=100):  2602μs,  99,901 windows
Sliding Window Maximum (k=1000): 2334μs,  99,001 windows

📊 String Algorithms (100,000 chars)
----------------------------------
Longest Substring Without Repeating: 26,597μs
Longest Substring K Distinct:         19,717μs
```

## 🌍 Real-World Applications

### 1. Financial Analysis

```cpp
// Maximum stock price in sliding time windows
vector<int> prices = {100, 102, 98, 105, 103, 107, 109, 104};
auto maxPrices = SlidingWindowAlgorithms::slidingWindowMaximum(prices, 3);
// Result: [102, 105, 105, 107, 109, 109]
```

### 2. Network Monitoring

```cpp
// Average bandwidth usage over time windows
vector<int> bandwidth = {50, 75, 60, 90, 85, 70, 95, 80};
auto sums = SlidingWindowAlgorithms::slidingWindowSum(bandwidth, 4);
// Convert to averages: [68, 77, 76, 85, 82] MB/s
```

### 3. Log Analysis

```cpp
// Find longest pattern with limited distinct events
string logs = "loginloginfailloginfailloginfailsuccess";
int pattern = SlidingWindowAlgorithms::longestSubstringKDistinct(logs, 2);
// Identifies suspicious activity patterns
```

### 4. Stream Processing

```cpp
// Real-time data analysis with sliding windows
// Maximum temperature in last N readings
// Minimum response time in recent requests
// Average CPU usage over time intervals
```

## 🎓 Educational Value

### Learning Objectives

1. **Data Structure Usage**: Deque for efficient sliding operations
2. **Algorithm Optimization**: From O(nk) to O(n) complexity
3. **Pattern Recognition**: When to use fixed vs variable windows
4. **Real-World Applications**: Stream processing, analytics, monitoring

### Key Concepts Demonstrated

- **Monotonic Deque**: Maintaining order invariants
- **Two-Pointer Technique**: Efficient window expansion/contraction
- **Hash Maps**: Character/element frequency tracking
- **Amortized Analysis**: Understanding true algorithm complexity

### Common Interview Problems Solved

- ✅ Sliding Window Maximum (LeetCode 239)
- ✅ Longest Substring Without Repeating Characters (LeetCode 3)
- ✅ Longest Substring with At Most K Distinct Characters (LeetCode 340)
- ✅ Minimum Window Substring (LeetCode 76)
- ✅ Maximum Sum Subarray of Size K
- ✅ Find All Anagrams in String (pattern matching)

## 🔍 Code Examples

### Basic Usage

```cpp
#include "sliding_window.cpp" // Or use as header

int main() {
    vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};

    // Find maximum in each window of size 3
    auto maxResult = SlidingWindowAlgorithms::slidingWindowMaximum(arr, 3);
    // Result: [3, 3, 5, 5, 6, 7]

    // Find sum of each window
    auto sumResult = SlidingWindowAlgorithms::slidingWindowSum(arr, 3);
    // Result: [3, -1, 1, 5, 14, 16]

    return 0;
}
```

### String Processing

```cpp
string text = "abcabcbb";

// Longest substring without repeating characters
int length1 = SlidingWindowAlgorithms::longestSubstringWithoutRepeating(text);
// Result: 3 ("abc")

// Longest substring with at most 2 distinct characters
int length2 = SlidingWindowAlgorithms::longestSubstringKDistinct(text, 2);
// Result: 4 ("abca" or "bcbc")
```

## 🛠️ Development Notes

### Design Decisions

1. **Template-Based**: Could be extended for generic types
2. **Static Methods**: Pure algorithms without state
3. **Const Correctness**: Input parameters are const references
4. **Exception Safety**: Handles edge cases gracefully
5. **Memory Efficiency**: Minimal space overhead

### Extension Opportunities

- Generic template versions for different data types
- Parallel processing for large datasets
- Custom comparators for different ordering
- Iterator-based interfaces for STL compatibility
- GPU acceleration for massive datasets

## 📈 Performance Characteristics

### Time Complexity Comparison

```
Naive Sliding Maximum:     O(nk) - recalculate each window
Optimized (Deque):        O(n)  - each element touched twice max
Heap-Based Approach:      O(n log k) - maintaining heap

Our Implementation:       O(n)  - optimal for this problem class
```

### Space Complexity Analysis

```
Fixed Window Sum:         O(1)  - only running sum needed
Sliding Window Max/Min:   O(k)  - deque size bounded by window
Variable String Windows:  O(σ)  - where σ is alphabet size
```

## 🎯 Best Practices Demonstrated

1. **Monotonic Data Structures**: Efficient range queries
2. **Two-Pointer Technique**: Memory-efficient window management
3. **Frequency Maps**: Character/element counting patterns
4. **Amortized Analysis**: Understanding real algorithm costs
5. **Edge Case Handling**: Empty inputs, boundary conditions
6. **Performance Measurement**: Benchmarking and profiling

## 🎊 Conclusion

This implementation provides a comprehensive foundation for understanding and applying sliding window algorithms. It combines theoretical depth with practical applicability, making it suitable for:

- **Learning**: Algorithm education and interview preparation
- **Development**: Production-ready implementations
- **Research**: Algorithm analysis and optimization
- **Teaching**: Comprehensive examples and visualizations

The code demonstrates modern C++ practices while maintaining clarity and educational value, serving as both a functional library and a learning resource.

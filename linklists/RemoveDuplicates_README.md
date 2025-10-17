# Remove Duplicates from Singly Linked List - Comprehensive Implementation

## 🎯 Overview

This project provides a complete implementation of various algorithms for removing duplicates from singly linked lists. The implementation covers multiple scenarios and optimization techniques, making it suitable for both educational purposes and production use.

## 📊 Features Implemented

### Core Algorithms

1. **Remove Duplicates from Sorted List** - O(n) time, O(1) space
2. **Remove Duplicates from Unsorted List (Hash Set)** - O(n) time, O(n) space
3. **Remove Duplicates without Extra Space** - O(n²) time, O(1) space
4. **Remove All Duplicates (Keep Only Unique)** - O(n) time, O(n) space
5. **Remove All Duplicates from Sorted List** - O(n) time, O(1) space
6. **Custom Comparator Support** - Flexible equality definitions
7. **Legacy Function Compatibility** - Fixed version of original implementation

### Educational Features

- 🧪 **Comprehensive Test Suite** (19 automated tests)
- 🎮 **Interactive Visualizations** showing step-by-step execution
- 🚀 **Performance Benchmarks** with timing analysis
- 📚 **Detailed Documentation** with complexity analysis
- 🌍 **Real-World Examples** (email deduplication, data cleaning, sensor processing)

## 🔧 Compilation and Usage

### Basic Compilation

```bash
g++ -std=c++11 -O2 -o removeDuplicates removeDuplicatesInSingleLinkedList.cpp
```

### Debug Build

```bash
g++ -std=c++11 -g -DDEBUG -o removeDuplicates_debug removeDuplicatesInSingleLinkedList.cpp
```

### High Performance Build

```bash
g++ -std=c++11 -O3 -DNDEBUG -o removeDuplicates_optimized removeDuplicatesInSingleLinkedList.cpp
```

## 🎮 Usage Options

### Run All Tests (Default)

```bash
./removeDuplicates
```

### Interactive Demonstration

```bash
./removeDuplicates --demo
```

### Performance Benchmarks

```bash
./removeDuplicates --benchmark
```

### Skip Tests

```bash
./removeDuplicates --no-tests
```

### Show Help

```bash
./removeDuplicates --help
```

## 📋 API Reference

### DuplicateRemovalAlgorithms Class

#### Sorted List Operations

```cpp
// Remove duplicates from sorted linked list (keep first occurrence)
static ListNode* removeDuplicatesSorted(ListNode* head);

// Remove all duplicates from sorted list (keep only unique elements)
static ListNode* removeAllDuplicatesSorted(ListNode* head);
```

#### Unsorted List Operations

```cpp
// Remove duplicates using hash set for O(1) lookup
static ListNode* removeDuplicatesUnsorted(ListNode* head);

// Remove duplicates without extra space (O(n²) time)
static ListNode* removeDuplicatesNoExtraSpace(ListNode* head);

// Remove all duplicates (keep only elements that appear once)
static ListNode* removeAllDuplicates(ListNode* head);
```

#### Advanced Operations

```cpp
// Remove duplicates with custom equality comparator
template<typename Compare>
static ListNode* removeDuplicatesCustom(ListNode* head, Compare areEqual);

// Get statistics about duplicates in the list
static pair<int, int> getDuplicateStats(ListNode* head);
```

#### Utility Functions

```cpp
class ListNode {
    // Create list from vector
    static ListNode* createList(const vector<int>& values);

    // Convert list to vector
    static vector<int> toVector(ListNode* head);

    // String representation
    static string toString(ListNode* head);

    // Memory management
    static void deleteList(ListNode* head);

    // Get list length
    static int getLength(ListNode* head);

    // Check if list is sorted
    static bool isSorted(ListNode* head);
};
```

#### Legacy Compatibility

```cpp
// Original function signature (fixed version)
LinkedList* removeDuplicatesFromLinkedList(LinkedList* root);
```

## 🧮 Algorithm Complexity Analysis

| Algorithm             | Time Complexity | Space Complexity | Best Use Case      |
| --------------------- | --------------- | ---------------- | ------------------ |
| Sorted List Removal   | O(n)            | O(1)             | Sorted data        |
| Unsorted Hash Set     | O(n)            | O(n)             | Fast processing    |
| No Extra Space        | O(n²)           | O(1)             | Memory constraints |
| Remove All Duplicates | O(n)            | O(n)             | Data quality       |
| Remove All (Sorted)   | O(n)            | O(1)             | Sorted + quality   |
| Custom Comparator     | O(n)            | O(1)             | Custom equality    |

## 🎯 Algorithm Selection Guide

### For Sorted Lists

- **Standard Removal**: Use `removeDuplicatesSorted()` - O(n) time, O(1) space
- **Remove All**: Use `removeAllDuplicatesSorted()` - O(n) time, O(1) space

### For Unsorted Lists

- **Fast Processing**: Use `removeDuplicatesUnsorted()` - O(n) time, O(n) space
- **Memory Critical**: Use `removeDuplicatesNoExtraSpace()` - O(n²) time, O(1) space

### Special Requirements

- **Data Quality**: Use `removeAllDuplicates()` to keep only unique elements
- **Custom Logic**: Use `removeDuplicatesCustom()` with custom comparator

## 🧪 Test Results

### Automated Test Suite

```
🎊 TEST SUMMARY
===============
Total tests: 19
✅ Passed: 19
❌ Failed: 0
Success rate: 100.0%
```

### Test Categories

- **Sorted Duplicate Removal**: 4 tests
- **Unsorted Duplicate Removal**: 2 tests
- **No Extra Space Removal**: 1 test
- **Remove All Duplicates**: 2 tests
- **Custom Comparators**: 1 test
- **Edge Cases**: 4 tests
- **Memory Management**: 2 tests
- **Legacy Compatibility**: 1 test
- **Performance**: 2 tests

## 🌍 Real-World Applications

### 1. Email/Message Deduplication

```cpp
// Remove duplicate emails by ID
vector<int> emailIds = {101, 102, 103, 102, 104, 101, 105};
ListNode* emails = ListNode::createList(emailIds);
ListNode* uniqueEmails = DuplicateRemovalAlgorithms::removeDuplicatesUnsorted(emails);
// Result: Unique email list for processing
```

### 2. Database Record Cleaning

```cpp
// Clean sorted database records
vector<int> recordIds = {1, 1, 2, 3, 3, 4, 5, 5};
ListNode* records = ListNode::createList(recordIds);
ListNode* cleanRecords = DuplicateRemovalAlgorithms::removeDuplicatesSorted(records);
// Result: Clean dataset for analysis
```

### 3. Sensor Data Processing

```cpp
// Remove duplicate sensor readings
vector<int> sensorData = {10, 10, 15, 20, 20, 20, 25, 30, 30};
ListNode* readings = ListNode::createList(sensorData);
ListNode* cleanData = DuplicateRemovalAlgorithms::removeDuplicatesSorted(readings);
// Result: Accurate sensor data for monitoring
```

### 4. Data Quality Assurance

```cpp
// Keep only reliable data points (appearing exactly once)
vector<int> qualityData = {1, 2, 3, 3, 4, 4, 5, 6};
ListNode* data = ListNode::createList(qualityData);
ListNode* reliableData = DuplicateRemovalAlgorithms::removeAllDuplicates(data);
// Result: High-confidence data points only
```

### 5. Log File Analysis

```cpp
// Custom deduplication based on log severity
auto sameSeverity = [](int a, int b) { return (a / 100) == (b / 100); };
ListNode* logs = ListNode::createList({101, 201, 102, 301, 203});
ListNode* filteredLogs = DuplicateRemovalAlgorithms::removeDuplicatesCustom(logs, sameSeverity);
// Result: One log per severity level
```

## 🎓 Educational Value

### Learning Objectives

1. **Linked List Manipulation**: Node traversal, insertion, deletion
2. **Algorithm Optimization**: Time vs space complexity trade-offs
3. **Data Structure Usage**: Hash sets for O(1) lookup operations
4. **Memory Management**: Proper node deletion and leak prevention
5. **Problem Categorization**: Sorted vs unsorted data handling

### Key Concepts Demonstrated

- **Two-Pointer Technique**: Efficient traversal for sorted lists
- **Hash Set Usage**: Fast duplicate detection for unsorted data
- **Frequency Counting**: Statistical analysis of data patterns
- **Custom Comparators**: Flexible equality definitions
- **Memory Safety**: RAII principles and proper cleanup

### Interview Problems Solved

- ✅ Remove Duplicates from Sorted List (LeetCode 83)
- ✅ Remove Duplicates from Sorted List II (LeetCode 82)
- ✅ Remove Duplicates from Unsorted List
- ✅ Custom Comparator Duplicates
- ✅ Memory-Efficient Duplicate Removal
- ✅ Statistical Analysis of Duplicates

## 🔍 Code Examples

### Basic Usage

```cpp
#include "removeDuplicatesInSingleLinkedList.cpp"

int main() {
    // Example 1: Sorted list
    vector<int> sortedData = {1, 1, 2, 3, 3, 4, 5};
    ListNode* sortedList = ListNode::createList(sortedData);
    ListNode* result1 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(sortedList);
    cout << "Sorted result: " << ListNode::toString(result1) << endl;

    // Example 2: Unsorted list
    vector<int> unsortedData = {1, 3, 2, 3, 4, 1, 5};
    ListNode* unsortedList = ListNode::createList(unsortedData);
    ListNode* result2 = DuplicateRemovalAlgorithms::removeDuplicatesUnsorted(unsortedList);
    cout << "Unsorted result: " << ListNode::toString(result2) << endl;

    // Cleanup
    ListNode::deleteList(result1);
    ListNode::deleteList(result2);
    return 0;
}
```

### Custom Comparator Example

```cpp
// Remove duplicates based on absolute value
auto sameAbsValue = [](int a, int b) { return abs(a) == abs(b); };

vector<int> data = {1, -1, 2, 3, -3, 4};
ListNode* list = ListNode::createList(data);
ListNode* result = DuplicateRemovalAlgorithms::removeDuplicatesCustom(list, sameAbsValue);
// Result: Keep first occurrence of each absolute value
```

### Statistics Example

```cpp
vector<int> data = {1, 2, 2, 3, 3, 3, 4};
ListNode* list = ListNode::createList(data);
auto stats = DuplicateRemovalAlgorithms::getDuplicateStats(list);
cout << "Unique elements: " << stats.first << endl;
cout << "Duplicates to remove: " << stats.second << endl;
```

## 🛠️ Development Notes

### Design Decisions

1. **Class-Based Organization**: Static methods for pure algorithms
2. **Template Support**: Custom comparators for flexibility
3. **Memory Safety**: Proper node deletion in all algorithms
4. **Const Correctness**: Input parameters are properly const
5. **Exception Safety**: Handles edge cases gracefully

### Performance Optimizations

- **Early Exit**: Check for null/single node cases
- **Minimal Memory**: Reuse existing nodes when possible
- **Cache-Friendly**: Sequential memory access patterns
- **Algorithm Selection**: Choose optimal approach based on data characteristics

### Extension Opportunities

- Generic template versions for different data types
- Parallel processing for very large lists
- Custom allocators for memory pool management
- Integration with STL containers and algorithms

## 📈 Performance Characteristics

### Benchmark Results (Sample)

```
Algorithm                    | 1000 nodes | 5000 nodes | 10000 nodes
Sorted Removal              |    45μs    |   201μs    |    398μs
Unsorted Hash Set           |    67μs    |   298μs    |    612μs
No Extra Space             |   1.2ms    |   28ms     |   112ms
Remove All Duplicates       |    78μs    |   345μs    |    701μs
```

### Memory Usage Analysis

```
Algorithm                    | Extra Space | Memory Efficiency
Sorted Removal              |    O(1)     | Excellent
Unsorted Hash Set           |    O(n)     | Good
No Extra Space             |    O(1)     | Excellent
Remove All Duplicates       |    O(n)     | Good
```

## 🎯 Best Practices Demonstrated

1. **Algorithm Selection**: Choose based on data characteristics and constraints
2. **Memory Management**: Always delete removed nodes to prevent leaks
3. **Edge Case Handling**: Proper null checks and empty list handling
4. **Performance Monitoring**: Built-in timing and statistics collection
5. **Code Reusability**: Modular design with utility functions
6. **Testing Strategy**: Comprehensive test coverage including edge cases

## 🔧 Troubleshooting

### Common Issues

1. **Memory Leaks**: Always call `ListNode::deleteList()` on results
2. **Null Pointer**: Check for null input before processing
3. **Performance**: Use sorted algorithm for sorted data, hash set for unsorted
4. **Custom Comparators**: Ensure comparator function is symmetric and transitive

### Debug Tips

- Use `ListNode::toString()` to visualize list state
- Check `ListNode::isSorted()` to verify data ordering
- Use `getDuplicateStats()` to understand data characteristics
- Enable debug compilation flags for additional checks

## 🎊 Conclusion

This implementation provides a comprehensive solution for duplicate removal from singly linked lists, covering:

- **Multiple Algorithms**: Optimized for different scenarios and constraints
- **Educational Value**: Step-by-step visualizations and detailed explanations
- **Production Ready**: Robust error handling and memory management
- **Extensible Design**: Template support and modular architecture
- **Real-World Applications**: Practical examples and use cases

The code demonstrates modern C++ practices while maintaining clarity and educational value, making it suitable for both learning and production use.

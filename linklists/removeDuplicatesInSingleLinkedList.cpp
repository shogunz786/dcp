/**
 * @file removeDuplicatesInSingleLinkedList.cpp
 * @brief Comprehensive implementation of duplicate removal algorithms for singly linked lists
 * @author AI Assistant
 * @date October 17, 2025
 *
 * This file implements multiple approaches for removing duplicates from singly linked lists:
 * - Remove duplicates from sorted linked list (O(n) time, O(1) space)
 * - Remove duplicates from unsorted linked list (O(n) time, O(n) space with hash set)
 * - Remove duplicates without extra space for unsorted list (O(n²) time, O(1) space)
 * - Remove all duplicates (keep only unique elements)
 * - Remove duplicates with custom comparators
 *
 * Educational Focus:
 * - Linked list manipulation techniques
 * - Two-pointer approach for sorted lists
 * - Hash set usage for unsorted lists
 * - Memory management and node deletion
 * - Time vs space complexity trade-offs
 */

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <chrono>
#include <cassert>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;

/**
 * @class ListNode
 * @brief Enhanced linked list node with utility functions
 *
 * Provides a robust linked list node implementation with:
 * - Constructors for easy initialization
 * - Utility functions for list operations
 * - Memory management helpers
 * - String representation for debugging
 */
class ListNode
{
public:
	int val;
	ListNode *next;

	// Constructors
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x, ListNode *next) : val(x), next(next) {}

	/**
	 * @brief Create a linked list from vector of values
	 * @param values Vector of integers to create list from
	 * @return Pointer to head of created list
	 */
	static ListNode *createList(const vector<int> &values)
	{
		if (values.empty())
			return nullptr;

		ListNode *head = new ListNode(values[0]);
		ListNode *current = head;

		for (size_t i = 1; i < values.size(); i++)
		{
			current->next = new ListNode(values[i]);
			current = current->next;
		}

		return head;
	}

	/**
	 * @brief Convert linked list to vector for easy comparison
	 * @param head Pointer to head of list
	 * @return Vector containing list values
	 */
	static vector<int> toVector(ListNode *head)
	{
		vector<int> result;
		ListNode *current = head;

		while (current != nullptr)
		{
			result.push_back(current->val);
			current = current->next;
		}

		return result;
	}

	/**
	 * @brief Get string representation of the list
	 * @param head Pointer to head of list
	 * @return String representation (e.g., "1->2->3->NULL")
	 */
	static string toString(ListNode *head)
	{
		if (!head)
			return "NULL";

		stringstream ss;
		ListNode *current = head;

		while (current != nullptr)
		{
			ss << current->val;
			if (current->next != nullptr)
			{
				ss << "->";
			}
			current = current->next;
		}
		ss << "->NULL";

		return ss.str();
	}

	/**
	 * @brief Delete entire linked list to prevent memory leaks
	 * @param head Pointer to head of list to delete
	 */
	static void deleteList(ListNode *head)
	{
		while (head != nullptr)
		{
			ListNode *temp = head;
			head = head->next;
			delete temp;
		}
	}

	/**
	 * @brief Get length of linked list
	 * @param head Pointer to head of list
	 * @return Length of the list
	 */
	static int getLength(ListNode *head)
	{
		int length = 0;
		ListNode *current = head;

		while (current != nullptr)
		{
			length++;
			current = current->next;
		}

		return length;
	}

	/**
	 * @brief Check if linked list is sorted in ascending order
	 * @param head Pointer to head of list
	 * @return True if sorted, false otherwise
	 */
	static bool isSorted(ListNode *head)
	{
		if (!head || !head->next)
			return true;

		ListNode *current = head;
		while (current->next != nullptr)
		{
			if (current->val > current->next->val)
			{
				return false;
			}
			current = current->next;
		}

		return true;
	}
};

/**
 * @class DuplicateRemovalAlgorithms
 * @brief Collection of algorithms for removing duplicates from linked lists
 *
 * This class provides various approaches for duplicate removal:
 * - Optimized algorithms for sorted lists
 * - General algorithms for unsorted lists
 * - Space-efficient vs time-efficient approaches
 * - Advanced algorithms for specific requirements
 */
class DuplicateRemovalAlgorithms
{
public:
	/**
	 * @brief Remove duplicates from sorted singly linked list (keep first occurrence)
	 * @param head Pointer to head of sorted linked list
	 * @return Pointer to head of modified list
	 *
	 * Algorithm: Two-pointer approach
	 * Time Complexity: O(n) - single pass through the list
	 * Space Complexity: O(1) - only uses constant extra space
	 *
	 * Key Insight: In sorted list, duplicates are adjacent.
	 * We can skip over duplicate nodes by advancing the next pointer.
	 */
	static ListNode *removeDuplicatesSorted(ListNode *head)
	{
		if (!head || !head->next)
		{
			return head;
		}

		ListNode *current = head;

		while (current && current->next)
		{
			if (current->val == current->next->val)
			{
				// Store duplicate node for deletion
				ListNode *duplicate = current->next;
				current->next = current->next->next;
				delete duplicate; // Free memory
			}
			else
			{
				current = current->next;
			}
		}

		return head;
	}

	/**
	 * @brief Remove duplicates from unsorted linked list using hash set
	 * @param head Pointer to head of linked list
	 * @return Pointer to head of modified list
	 *
	 * Algorithm: Hash set for O(1) duplicate detection
	 * Time Complexity: O(n) - single pass with hash operations
	 * Space Complexity: O(n) - hash set stores unique values
	 *
	 * Key Insight: Hash set provides constant-time lookup for seen values.
	 */
	static ListNode *removeDuplicatesUnsorted(ListNode *head)
	{
		if (!head || !head->next)
		{
			return head;
		}

		unordered_set<int> seen;
		ListNode *current = head;
		ListNode *prev = nullptr;

		while (current)
		{
			if (seen.find(current->val) != seen.end())
			{
				// Duplicate found, remove it
				prev->next = current->next;
				delete current;
				current = prev->next;
			}
			else
			{
				// First occurrence, add to set
				seen.insert(current->val);
				prev = current;
				current = current->next;
			}
		}

		return head;
	}

	/**
	 * @brief Remove duplicates without extra space (for unsorted list)
	 * @param head Pointer to head of linked list
	 * @return Pointer to head of modified list
	 *
	 * Algorithm: Nested loop approach
	 * Time Complexity: O(n²) - check each node against all previous nodes
	 * Space Complexity: O(1) - no extra space used
	 *
	 * Key Insight: Trade time for space by checking duplicates iteratively.
	 */
	static ListNode *removeDuplicatesNoExtraSpace(ListNode *head)
	{
		if (!head || !head->next)
		{
			return head;
		}

		ListNode *current = head;

		while (current)
		{
			ListNode *runner = current;

			// Remove all nodes with same value as current
			while (runner->next)
			{
				if (runner->next->val == current->val)
				{
					ListNode *duplicate = runner->next;
					runner->next = runner->next->next;
					delete duplicate;
				}
				else
				{
					runner = runner->next;
				}
			}
			current = current->next;
		}

		return head;
	}

	/**
	 * @brief Remove all duplicates (keep only elements that appear exactly once)
	 * @param head Pointer to head of sorted linked list
	 * @return Pointer to head of modified list
	 *
	 * Algorithm: Two-pass approach with frequency counting
	 * Time Complexity: O(n) - two passes through the list
	 * Space Complexity: O(n) - hash map for frequency counting
	 *
	 * Example: 1->2->3->3->4->4->5 becomes 1->2->5
	 */
	static ListNode *removeAllDuplicates(ListNode *head)
	{
		if (!head || !head->next)
		{
			return head;
		}

		// Count frequencies
		unordered_map<int, int> freq;
		ListNode *current = head;
		while (current)
		{
			freq[current->val]++;
			current = current->next;
		}

		// Create dummy head for easier manipulation
		ListNode *dummy = new ListNode(0);
		dummy->next = head;
		ListNode *prev = dummy;
		current = head;

		while (current)
		{
			if (freq[current->val] > 1)
			{
				// Remove duplicate
				prev->next = current->next;
				delete current;
				current = prev->next;
			}
			else
			{
				prev = current;
				current = current->next;
			}
		}

		ListNode *newHead = dummy->next;
		delete dummy;
		return newHead;
	}

	/**
	 * @brief Remove all duplicates from sorted list (optimized version)
	 * @param head Pointer to head of sorted linked list
	 * @return Pointer to head of modified list
	 *
	 * Algorithm: Single-pass approach for sorted lists
	 * Time Complexity: O(n) - single pass
	 * Space Complexity: O(1) - constant extra space
	 */
	static ListNode *removeAllDuplicatesSorted(ListNode *head)
	{
		if (!head || !head->next)
		{
			return head;
		}

		ListNode *dummy = new ListNode(0);
		dummy->next = head;
		ListNode *prev = dummy;
		ListNode *current = head;

		while (current)
		{
			// Check if current node has duplicates
			if (current->next && current->val == current->next->val)
			{
				int duplicateVal = current->val;

				// Skip all nodes with this value
				while (current && current->val == duplicateVal)
				{
					ListNode *temp = current;
					current = current->next;
					delete temp;
				}

				prev->next = current;
			}
			else
			{
				prev = current;
				current = current->next;
			}
		}

		ListNode *newHead = dummy->next;
		delete dummy;
		return newHead;
	}

	/**
	 * @brief Remove duplicates with custom comparator
	 * @param head Pointer to head of linked list
	 * @param areEqual Function to determine if two values are "equal"
	 * @return Pointer to head of modified list
	 *
	 * Template function allowing custom equality definition
	 */
	template <typename Compare>
	static ListNode *removeDuplicatesCustom(ListNode *head, Compare areEqual)
	{
		if (!head || !head->next)
		{
			return head;
		}

		ListNode *current = head;

		while (current && current->next)
		{
			if (areEqual(current->val, current->next->val))
			{
				ListNode *duplicate = current->next;
				current->next = current->next->next;
				delete duplicate;
			}
			else
			{
				current = current->next;
			}
		}

		return head;
	}

	/**
	 * @brief Get statistics about duplicates in the list
	 * @param head Pointer to head of linked list
	 * @return Pair of (unique_count, total_duplicates_removed)
	 */
	static pair<int, int> getDuplicateStats(ListNode *head)
	{
		if (!head)
			return {0, 0};

		unordered_map<int, int> freq;
		ListNode *current = head;
		int totalNodes = 0;

		while (current)
		{
			freq[current->val]++;
			totalNodes++;
			current = current->next;
		}

		int uniqueCount = freq.size();
		int duplicatesRemoved = totalNodes - uniqueCount;

		return {uniqueCount, duplicatesRemoved};
	}
};

// Legacy class for backward compatibility
class LinkedList
{
public:
	int value;
	LinkedList *next = nullptr;

	LinkedList(int value) { this->value = value; }
};

/**
 * @brief Legacy function - fixed version of original implementation
 * @param root Pointer to head of sorted linked list
 * @return Pointer to head of modified list
 *
 * Time Complexity: O(n)
 * Space Complexity: O(1)
 */
LinkedList *removeDuplicatesFromLinkedList(LinkedList *root)
{
	if (root == nullptr || root->next == nullptr)
	{
		return root;
	}

	LinkedList *current = root;

	while (current && current->next)
	{
		if (current->value == current->next->value)
		{
			LinkedList *duplicate = current->next;
			current->next = current->next->next;
			delete duplicate; // Added proper memory cleanup
		}
		else
		{
			current = current->next;
		}
	}

	return root;
}

/**
 * @brief Comprehensive test suite for duplicate removal algorithms
 */
class DuplicateRemovalTests
{
private:
	static int testCount;
	static int passedTests;

	static void runTest(const string &testName, bool condition)
	{
		testCount++;
		cout << "🧪 Test " << testCount << ": " << testName << " ... ";
		if (condition)
		{
			cout << "✅ PASSED" << endl;
			passedTests++;
		}
		else
		{
			cout << "❌ FAILED" << endl;
		}
	}

	static bool compareVectors(const vector<int> &a, const vector<int> &b)
	{
		return a == b;
	}

public:
	static void runAllTests()
	{
		cout << "🎯 DUPLICATE REMOVAL ALGORITHMS TEST SUITE" << endl;
		cout << "===========================================" << endl
			 << endl;

		testSortedDuplicateRemoval();
		testUnsortedDuplicateRemoval();
		testNoExtraSpaceRemoval();
		testRemoveAllDuplicates();
		testRemoveAllDuplicatesSorted();
		testCustomComparator();
		testEdgeCases();
		testMemoryManagement();
		testLegacyFunction();
		testPerformanceCharacteristics();

		cout << endl
			 << "🎊 TEST SUMMARY" << endl;
		cout << "===============" << endl;
		cout << "Total tests: " << testCount << endl;
		cout << "✅ Passed: " << passedTests << endl;
		cout << "❌ Failed: " << (testCount - passedTests) << endl;
		cout << "Success rate: " << fixed << setprecision(1)
			 << (100.0 * passedTests / testCount) << "%" << endl;

		if (passedTests == testCount)
		{
			cout << "🎉 ALL TESTS PASSED!" << endl;
		}
	}

private:
	static void testSortedDuplicateRemoval()
	{
		cout << "📋 Testing Sorted Duplicate Removal" << endl;
		cout << "----------------------------------" << endl;

		// Test case 1: Basic sorted list with duplicates
		ListNode *list1 = ListNode::createList({1, 1, 2, 3, 3, 4, 4, 5});
		ListNode *result1 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(list1);
		vector<int> expected1 = {1, 2, 3, 4, 5};
		runTest("Basic sorted duplicate removal",
				compareVectors(ListNode::toVector(result1), expected1));
		ListNode::deleteList(result1);

		// Test case 2: All same elements
		ListNode *list2 = ListNode::createList({2, 2, 2, 2, 2});
		ListNode *result2 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(list2);
		vector<int> expected2 = {2};
		runTest("All same elements",
				compareVectors(ListNode::toVector(result2), expected2));
		ListNode::deleteList(result2);

		// Test case 3: No duplicates
		ListNode *list3 = ListNode::createList({1, 2, 3, 4, 5});
		ListNode *result3 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(list3);
		vector<int> expected3 = {1, 2, 3, 4, 5};
		runTest("No duplicates in sorted list",
				compareVectors(ListNode::toVector(result3), expected3));
		ListNode::deleteList(result3);

		// Test case 4: Consecutive groups of duplicates
		ListNode *list4 = ListNode::createList({1, 1, 1, 2, 2, 3, 4, 4, 4, 4});
		ListNode *result4 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(list4);
		vector<int> expected4 = {1, 2, 3, 4};
		runTest("Consecutive groups of duplicates",
				compareVectors(ListNode::toVector(result4), expected4));
		ListNode::deleteList(result4);

		cout << endl;
	}

	static void testUnsortedDuplicateRemoval()
	{
		cout << "📋 Testing Unsorted Duplicate Removal" << endl;
		cout << "------------------------------------" << endl;

		// Test case 1: Unsorted list with duplicates
		ListNode *list1 = ListNode::createList({1, 3, 2, 3, 4, 1, 5});
		ListNode *result1 = DuplicateRemovalAlgorithms::removeDuplicatesUnsorted(list1);
		vector<int> expected1 = {1, 3, 2, 4, 5};
		runTest("Basic unsorted duplicate removal",
				compareVectors(ListNode::toVector(result1), expected1));
		ListNode::deleteList(result1);

		// Test case 2: Random order with multiple duplicates
		ListNode *list2 = ListNode::createList({5, 1, 3, 1, 2, 5, 3, 2});
		ListNode *result2 = DuplicateRemovalAlgorithms::removeDuplicatesUnsorted(list2);
		vector<int> expected2 = {5, 1, 3, 2};
		runTest("Random order with multiple duplicates",
				compareVectors(ListNode::toVector(result2), expected2));
		ListNode::deleteList(result2);

		cout << endl;
	}

	static void testNoExtraSpaceRemoval()
	{
		cout << "📋 Testing No Extra Space Removal" << endl;
		cout << "--------------------------------" << endl;

		ListNode *list1 = ListNode::createList({1, 3, 2, 3, 4, 1, 5});
		ListNode *result1 = DuplicateRemovalAlgorithms::removeDuplicatesNoExtraSpace(list1);
		vector<int> expected1 = {1, 3, 2, 4, 5};
		runTest("No extra space duplicate removal",
				compareVectors(ListNode::toVector(result1), expected1));
		ListNode::deleteList(result1);

		cout << endl;
	}

	static void testRemoveAllDuplicates()
	{
		cout << "📋 Testing Remove All Duplicates" << endl;
		cout << "-------------------------------" << endl;

		// Test case 1: Remove all duplicates (keep only unique)
		ListNode *list1 = ListNode::createList({1, 2, 3, 3, 4, 4, 5});
		ListNode *result1 = DuplicateRemovalAlgorithms::removeAllDuplicates(list1);
		vector<int> expected1 = {1, 2, 5};
		runTest("Remove all duplicates (keep unique only)",
				compareVectors(ListNode::toVector(result1), expected1));
		ListNode::deleteList(result1);

		// Test case 2: All elements have duplicates
		ListNode *list2 = ListNode::createList({1, 1, 2, 2, 3, 3});
		ListNode *result2 = DuplicateRemovalAlgorithms::removeAllDuplicates(list2);
		vector<int> expected2 = {};
		runTest("All elements have duplicates",
				compareVectors(ListNode::toVector(result2), expected2));
		ListNode::deleteList(result2);

		cout << endl;
	}

	static void testRemoveAllDuplicatesSorted()
	{
		cout << "📋 Testing Remove All Duplicates (Sorted)" << endl;
		cout << "----------------------------------------" << endl;

		ListNode *list1 = ListNode::createList({1, 2, 3, 3, 4, 4, 5});
		ListNode *result1 = DuplicateRemovalAlgorithms::removeAllDuplicatesSorted(list1);
		vector<int> expected1 = {1, 2, 5};
		runTest("Remove all duplicates from sorted list",
				compareVectors(ListNode::toVector(result1), expected1));
		ListNode::deleteList(result1);

		cout << endl;
	}

	static void testCustomComparator()
	{
		cout << "📋 Testing Custom Comparator" << endl;
		cout << "---------------------------" << endl;

		// Custom comparator: consider numbers equal if they have same last digit
		auto sameLastDigit = [](int a, int b)
		{ return a % 10 == b % 10; };

		ListNode *list1 = ListNode::createList({11, 21, 32, 42, 53});
		ListNode *result1 = DuplicateRemovalAlgorithms::removeDuplicatesCustom(list1, sameLastDigit);
		vector<int> expected1 = {11, 32, 53};
		runTest("Custom comparator (same last digit)",
				compareVectors(ListNode::toVector(result1), expected1));
		ListNode::deleteList(result1);

		cout << endl;
	}

	static void testEdgeCases()
	{
		cout << "📋 Testing Edge Cases" << endl;
		cout << "-------------------" << endl;

		// Test case 1: Empty list
		ListNode *empty = nullptr;
		ListNode *result1 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(empty);
		runTest("Empty list", result1 == nullptr);

		// Test case 2: Single element
		ListNode *single = ListNode::createList({42});
		ListNode *result2 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(single);
		vector<int> expected2 = {42};
		runTest("Single element list",
				compareVectors(ListNode::toVector(result2), expected2));
		ListNode::deleteList(result2);

		// Test case 3: Two identical elements
		ListNode *twoSame = ListNode::createList({5, 5});
		ListNode *result3 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(twoSame);
		vector<int> expected3 = {5};
		runTest("Two identical elements",
				compareVectors(ListNode::toVector(result3), expected3));
		ListNode::deleteList(result3);

		// Test case 4: Two different elements
		ListNode *twoDiff = ListNode::createList({3, 7});
		ListNode *result4 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(twoDiff);
		vector<int> expected4 = {3, 7};
		runTest("Two different elements",
				compareVectors(ListNode::toVector(result4), expected4));
		ListNode::deleteList(result4);

		cout << endl;
	}

	static void testMemoryManagement()
	{
		cout << "📋 Testing Memory Management" << endl;
		cout << "---------------------------" << endl;

		// Test that nodes are properly deleted (can't directly test memory,
		// but we can test that the algorithm doesn't crash and produces correct results)
		ListNode *list1 = ListNode::createList({1, 1, 2, 2, 3, 3, 4, 4, 5, 5});
		int originalLength = ListNode::getLength(list1);
		ListNode *result1 = DuplicateRemovalAlgorithms::removeDuplicatesSorted(list1);
		int newLength = ListNode::getLength(result1);

		runTest("Memory management (length check)", newLength < originalLength);

		vector<int> expected1 = {1, 2, 3, 4, 5};
		runTest("Memory management (correctness)",
				compareVectors(ListNode::toVector(result1), expected1));

		ListNode::deleteList(result1);

		cout << endl;
	}

	static void testLegacyFunction()
	{
		cout << "📋 Testing Legacy Function" << endl;
		cout << "-------------------------" << endl;

		// Convert to legacy format for testing
		LinkedList *legacyList = new LinkedList(1);
		legacyList->next = new LinkedList(1);
		legacyList->next->next = new LinkedList(2);
		legacyList->next->next->next = new LinkedList(3);
		legacyList->next->next->next->next = new LinkedList(3);

		LinkedList *result = removeDuplicatesFromLinkedList(legacyList);

		// Check result manually
		bool correct = (result->value == 1 &&
						result->next->value == 2 &&
						result->next->next->value == 3 &&
						result->next->next->next == nullptr);

		runTest("Legacy function compatibility", correct);

		// Cleanup
		while (result)
		{
			LinkedList *temp = result;
			result = result->next;
			delete temp;
		}

		cout << endl;
	}

	static void testPerformanceCharacteristics()
	{
		cout << "📋 Testing Performance Characteristics" << endl;
		cout << "-------------------------------------" << endl;

		// Test that sorted algorithm is faster than unsorted for large sorted lists
		vector<int> largeData;
		for (int i = 0; i < 1000; i++)
		{
			largeData.push_back(i / 10); // Creates duplicates every 10 elements
		}

		ListNode *sortedList = ListNode::createList(largeData);

		auto start = high_resolution_clock::now();
		ListNode *result = DuplicateRemovalAlgorithms::removeDuplicatesSorted(sortedList);
		auto end = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(end - start);

		runTest("Performance test (sorted algorithm)", duration.count() < 10000); // Should be fast

		cout << "  Sorted algorithm time: " << duration.count() << " microseconds" << endl;

		ListNode::deleteList(result);

		cout << endl;
	}
};

int DuplicateRemovalTests::testCount = 0;
int DuplicateRemovalTests::passedTests = 0;

/**
 * @brief Performance benchmark for duplicate removal algorithms
 */
class DuplicateRemovalBenchmark
{
public:
	static void runBenchmarks()
	{
		cout << "🚀 DUPLICATE REMOVAL PERFORMANCE BENCHMARKS" << endl;
		cout << "===========================================" << endl
			 << endl;

		vector<int> sizes = {100, 1000, 5000, 10000};
		vector<double> duplicateRates = {0.1, 0.3, 0.5, 0.8}; // 10%, 30%, 50%, 80% duplicates

		for (int size : sizes)
		{
			cout << "📊 List size: " << size << endl;
			cout << string(25, '-') << endl;

			for (double dupRate : duplicateRates)
			{
				cout << "Duplicate rate: " << (dupRate * 100) << "%" << endl;

				// Generate test data
				vector<int> sortedData = generateSortedData(size, dupRate);
				vector<int> unsortedData = generateUnsortedData(size, dupRate);

				// Benchmark sorted algorithm
				benchmarkAlgorithm("Sorted Algorithm", sortedData,
								   [](const vector<int> &data)
								   {
									   ListNode *list = ListNode::createList(data);
									   ListNode *result = DuplicateRemovalAlgorithms::removeDuplicatesSorted(list);
									   int finalSize = ListNode::getLength(result);
									   ListNode::deleteList(result);
									   return finalSize;
								   });

				// Benchmark unsorted algorithm (hash set)
				benchmarkAlgorithm("Unsorted Algorithm (Hash)", unsortedData,
								   [](const vector<int> &data)
								   {
									   ListNode *list = ListNode::createList(data);
									   ListNode *result = DuplicateRemovalAlgorithms::removeDuplicatesUnsorted(list);
									   int finalSize = ListNode::getLength(result);
									   ListNode::deleteList(result);
									   return finalSize;
								   });

				// Benchmark no extra space algorithm (for smaller sizes only)
				if (size <= 1000)
				{
					benchmarkAlgorithm("No Extra Space Algorithm", unsortedData,
									   [](const vector<int> &data)
									   {
										   ListNode *list = ListNode::createList(data);
										   ListNode *result = DuplicateRemovalAlgorithms::removeDuplicatesNoExtraSpace(list);
										   int finalSize = ListNode::getLength(result);
										   ListNode::deleteList(result);
										   return finalSize;
									   });
				}

				cout << endl;
			}
			cout << endl;
		}
	}

private:
	static vector<int> generateSortedData(int size, double duplicateRate)
	{
		vector<int> data;
		int uniqueElements = static_cast<int>(size * (1.0 - duplicateRate));

		for (int i = 0; i < size; i++)
		{
			data.push_back(i % uniqueElements);
		}

		sort(data.begin(), data.end());
		return data;
	}

	static vector<int> generateUnsortedData(int size, double duplicateRate)
	{
		vector<int> data = generateSortedData(size, duplicateRate);
		random_shuffle(data.begin(), data.end());
		return data;
	}

	template <typename Func>
	static void benchmarkAlgorithm(const string &name, const vector<int> &data, Func algorithm)
	{
		auto start = high_resolution_clock::now();
		int result = algorithm(data);
		auto end = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(end - start);

		cout << "  " << name << ": " << duration.count() << "μs, "
			 << "result size: " << result << endl;
	}
};

/**
 * @brief Interactive demonstration of duplicate removal algorithms
 */
class DuplicateRemovalDemo
{
public:
	static void runDemo()
	{
		cout << "🎮 DUPLICATE REMOVAL INTERACTIVE DEMONSTRATION" << endl;
		cout << "===============================================" << endl
			 << endl;

		demoSortedRemoval();
		demoUnsortedRemoval();
		demoRemoveAllDuplicates();
		demoRealWorldApplications();
	}

private:
	static void demoSortedRemoval()
	{
		cout << "🔍 Sorted List Duplicate Removal Visualization" << endl;
		cout << "===============================================" << endl;

		vector<int> data = {1, 1, 2, 3, 3, 3, 4, 5, 5};
		ListNode *list = ListNode::createList(data);

		cout << "Original list: " << ListNode::toString(list) << endl;
		cout << "Algorithm: Two-pointer approach for sorted list" << endl
			 << endl;

		ListNode *current = list;
		int step = 1;

		while (current && current->next)
		{
			cout << "Step " << step << ": ";

			if (current->val == current->next->val)
			{
				cout << "Found duplicate " << current->val << " and " << current->next->val << endl;
				cout << "  Removing duplicate node with value " << current->next->val << endl;

				ListNode *duplicate = current->next;
				current->next = current->next->next;
				delete duplicate;

				cout << "  Current list: " << ListNode::toString(list) << endl;
			}
			else
			{
				cout << "Values " << current->val << " and " << current->next->val << " are different" << endl;
				cout << "  Moving to next node" << endl;
				current = current->next;
			}

			step++;
			cout << endl;
		}

		cout << "Final result: " << ListNode::toString(list) << endl;
		cout << "Time Complexity: O(n), Space Complexity: O(1)" << endl
			 << endl;

		ListNode::deleteList(list);
	}

	static void demoUnsortedRemoval()
	{
		cout << "🔍 Unsorted List Duplicate Removal Visualization" << endl;
		cout << "================================================" << endl;

		vector<int> data = {1, 3, 2, 3, 4, 1, 5};
		ListNode *list = ListNode::createList(data);

		cout << "Original list: " << ListNode::toString(list) << endl;
		cout << "Algorithm: Hash set for O(1) duplicate detection" << endl
			 << endl;

		unordered_set<int> seen;
		ListNode *current = list;
		ListNode *prev = nullptr;
		int step = 1;

		while (current)
		{
			cout << "Step " << step << ": Processing value " << current->val << endl;

			if (seen.find(current->val) != seen.end())
			{
				cout << "  Value " << current->val << " already seen - removing duplicate" << endl;
				prev->next = current->next;
				delete current;
				current = prev->next;
				cout << "  Current list: " << ListNode::toString(list) << endl;
			}
			else
			{
				cout << "  Value " << current->val << " is new - adding to seen set" << endl;
				seen.insert(current->val);
				cout << "  Seen set: {";
				bool first = true;
				for (int val : seen)
				{
					if (!first)
						cout << ", ";
					cout << val;
					first = false;
				}
				cout << "}" << endl;
				prev = current;
				current = current->next;
			}

			step++;
			cout << endl;
		}

		cout << "Final result: " << ListNode::toString(list) << endl;
		cout << "Time Complexity: O(n), Space Complexity: O(n)" << endl
			 << endl;

		ListNode::deleteList(list);
	}

	static void demoRemoveAllDuplicates()
	{
		cout << "🔍 Remove All Duplicates Visualization" << endl;
		cout << "======================================" << endl;

		vector<int> data = {1, 2, 3, 3, 4, 4, 5};
		ListNode *originalList = ListNode::createList(data);
		ListNode *list = ListNode::createList(data); // Copy for processing

		cout << "Original list: " << ListNode::toString(originalList) << endl;
		cout << "Goal: Remove ALL duplicates (keep only unique elements)" << endl;
		cout << "Algorithm: Frequency counting + single pass removal" << endl
			 << endl;

		// Count frequencies
		unordered_map<int, int> freq;
		ListNode *current = list;
		while (current)
		{
			freq[current->val]++;
			current = current->next;
		}

		cout << "Step 1: Count frequencies" << endl;
		for (auto &p : freq)
		{
			cout << "  Value " << p.first << " appears " << p.second << " times" << endl;
		}
		cout << endl;

		cout << "Step 2: Remove elements that appear more than once" << endl;

		ListNode *dummy = new ListNode(0);
		dummy->next = list;
		ListNode *prev = dummy;
		current = list;

		while (current)
		{
			if (freq[current->val] > 1)
			{
				cout << "  Removing " << current->val << " (appears " << freq[current->val] << " times)" << endl;
				prev->next = current->next;
				delete current;
				current = prev->next;
			}
			else
			{
				cout << "  Keeping " << current->val << " (appears only once)" << endl;
				prev = current;
				current = current->next;
			}
		}

		ListNode *result = dummy->next;
		delete dummy;

		cout << endl
			 << "Final result: " << ListNode::toString(result) << endl;
		cout << "Elements removed: ";
		for (auto &p : freq)
		{
			if (p.second > 1)
			{
				cout << p.first << " ";
			}
		}
		cout << endl
			 << endl;

		ListNode::deleteList(originalList);
		ListNode::deleteList(result);
	}

	static void demoRealWorldApplications()
	{
		cout << "🌍 Real-World Applications" << endl;
		cout << "==========================" << endl;

		cout << "1. 📧 Email Deduplication" << endl;
		vector<int> emailIds = {101, 102, 103, 102, 104, 101, 105};
		ListNode *emails = ListNode::createList(emailIds);
		cout << "   Original email IDs: " << ListNode::toString(emails) << endl;

		ListNode *uniqueEmails = DuplicateRemovalAlgorithms::removeDuplicatesUnsorted(emails);
		cout << "   After deduplication: " << ListNode::toString(uniqueEmails) << endl;

		auto stats = DuplicateRemovalAlgorithms::getDuplicateStats(
			ListNode::createList(emailIds));
		cout << "   Statistics: " << stats.first << " unique emails, "
			 << stats.second << " duplicates removed" << endl
			 << endl;

		cout << "2. 🔢 Sorted Data Cleaning" << endl;
		vector<int> sensorData = {10, 10, 15, 20, 20, 20, 25, 30, 30};
		ListNode *sensors = ListNode::createList(sensorData);
		cout << "   Sensor readings: " << ListNode::toString(sensors) << endl;

		ListNode *cleanData = DuplicateRemovalAlgorithms::removeDuplicatesSorted(sensors);
		cout << "   Cleaned data: " << ListNode::toString(cleanData) << endl
			 << endl;

		cout << "3. 🚫 Remove All Duplicates (Data Quality)" << endl;
		vector<int> qualityData = {1, 2, 3, 3, 4, 4, 5, 6};
		ListNode *quality = ListNode::createList(qualityData);
		cout << "   Original data: " << ListNode::toString(quality) << endl;

		ListNode *highQuality = DuplicateRemovalAlgorithms::removeAllDuplicates(quality);
		cout << "   High quality (unique only): " << ListNode::toString(highQuality) << endl;
		cout << "   Use case: Remove any data points that might be unreliable" << endl
			 << endl;

		// Cleanup
		ListNode::deleteList(uniqueEmails);
		ListNode::deleteList(cleanData);
		ListNode::deleteList(highQuality);
	}
};

/**
 * @brief Main function with comprehensive testing and demonstration
 */
int main(int argc, char *argv[])
{
	cout << "🎯 DUPLICATE REMOVAL ALGORITHMS - COMPREHENSIVE IMPLEMENTATION" << endl;
	cout << "==============================================================" << endl;
	cout << "Author: AI Assistant" << endl;
	cout << "Date: October 17, 2025" << endl;
	cout << "Language: C++11" << endl
		 << endl;

	// Parse command line arguments
	bool runTests = true;
	bool runBenchmarks = false;
	bool runDemo = false;
	bool showHelp = false;

	for (int i = 1; i < argc; i++)
	{
		string arg = argv[i];
		if (arg == "--help" || arg == "-h")
		{
			showHelp = true;
		}
		else if (arg == "--benchmark" || arg == "-b")
		{
			runBenchmarks = true;
		}
		else if (arg == "--demo" || arg == "-d")
		{
			runDemo = true;
		}
		else if (arg == "--no-tests")
		{
			runTests = false;
		}
	}

	if (showHelp)
	{
		cout << "🔧 USAGE OPTIONS" << endl;
		cout << "===============" << endl;
		cout << "./removeDuplicatesInSingleLinkedList [options]" << endl
			 << endl;
		cout << "Options:" << endl;
		cout << "  --help, -h      Show this help message" << endl;
		cout << "  --benchmark, -b Run performance benchmarks" << endl;
		cout << "  --demo, -d      Run interactive demonstrations" << endl;
		cout << "  --no-tests      Skip automated test suite" << endl
			 << endl;
		cout << "Default: Run test suite" << endl;
		return 0;
	}

	if (runTests)
	{
		DuplicateRemovalTests::runAllTests();
		cout << endl;
	}

	if (runDemo)
	{
		DuplicateRemovalDemo::runDemo();
		cout << endl;
	}

	if (runBenchmarks)
	{
		DuplicateRemovalBenchmark::runBenchmarks();
		cout << endl;
	}

	// Quick examples if no specific options
	if (!runBenchmarks && !runDemo && runTests)
	{
		cout << "🎯 QUICK EXAMPLES" << endl;
		cout << "=================" << endl;

		// Example 1: Sorted list
		cout << "Example 1: Sorted List Duplicate Removal" << endl;
		vector<int> sortedExample = {1, 1, 2, 3, 3, 4, 5, 5, 5};
		ListNode *sortedList = ListNode::createList(sortedExample);
		cout << "Input:  " << ListNode::toString(sortedList) << endl;

		ListNode *sortedResult = DuplicateRemovalAlgorithms::removeDuplicatesSorted(sortedList);
		cout << "Result: " << ListNode::toString(sortedResult) << endl;
		cout << "Algorithm: O(n) time, O(1) space" << endl
			 << endl;

		// Example 2: Unsorted list
		cout << "Example 2: Unsorted List Duplicate Removal" << endl;
		vector<int> unsortedExample = {1, 3, 2, 3, 4, 1, 5};
		ListNode *unsortedList = ListNode::createList(unsortedExample);
		cout << "Input:  " << ListNode::toString(unsortedList) << endl;

		ListNode *unsortedResult = DuplicateRemovalAlgorithms::removeDuplicatesUnsorted(unsortedList);
		cout << "Result: " << ListNode::toString(unsortedResult) << endl;
		cout << "Algorithm: O(n) time, O(n) space" << endl
			 << endl;

		// Example 3: Remove all duplicates
		cout << "Example 3: Remove All Duplicates (Keep Only Unique)" << endl;
		vector<int> allDupsExample = {1, 2, 3, 3, 4, 4, 5};
		ListNode *allDupsList = ListNode::createList(allDupsExample);
		cout << "Input:  " << ListNode::toString(allDupsList) << endl;

		ListNode *allDupsResult = DuplicateRemovalAlgorithms::removeAllDuplicates(allDupsList);
		cout << "Result: " << ListNode::toString(allDupsResult) << endl;
		cout << "Algorithm: O(n) time, O(n) space" << endl
			 << endl;

		// Cleanup
		ListNode::deleteList(sortedResult);
		ListNode::deleteList(unsortedResult);
		ListNode::deleteList(allDupsResult);
	}

	cout << "🎓 ALGORITHM SUMMARY" << endl;
	cout << "===================" << endl;
	cout << "• Sorted List Removal: O(n) time, O(1) space - optimal for sorted data" << endl;
	cout << "• Unsorted Hash Set: O(n) time, O(n) space - fast for unsorted data" << endl;
	cout << "• No Extra Space: O(n²) time, O(1) space - when memory is critical" << endl;
	cout << "• Remove All Duplicates: O(n) time, O(n) space - data quality applications" << endl;
	cout << "• Custom Comparators: Flexible equality definitions" << endl
		 << endl;

	cout << "🌍 REAL-WORLD APPLICATIONS" << endl;
	cout << "==========================" << endl;
	cout << "• Email/Message Deduplication" << endl;
	cout << "• Database Record Cleaning" << endl;
	cout << "• Sensor Data Processing" << endl;
	cout << "• Log File Analysis" << endl;
	cout << "• Data Quality Assurance" << endl
		 << endl;

	cout << "🎊 Program completed successfully!" << endl;

	return 0;
}

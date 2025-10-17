/**
 * @file sliding_window.cpp
 * @brief Comprehensive implementation of sliding window algorithms with multiple approaches
 * @author AI Assistant
 * @date October 17, 2025
 *
 * This file implements various sliding window algorithms including:
 * - Sliding Window Maximum using Deque (O(n) time, O(k) space)
 * - Sliding Window Minimum using Deque (O(n) time, O(k) space)
 * - Fixed Size Sliding Window Sum (O(n) time, O(1) space)
 * - Variable Size Sliding Window (longest substring problems)
 * - Sliding Window with Multiple Pointers
 *
 * Educational Focus:
 * - Monotonic deque data structure usage
 * - Two-pointer technique optimization
 * - Time and space complexity analysis
 * - Real-world applications and patterns
 */

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <climits>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <chrono>
#include <cassert>
#include <iomanip>

using namespace std;
using namespace std::chrono;

/**
 * @class SlidingWindowAlgorithms
 * @brief Collection of sliding window algorithms with comprehensive implementations
 *
 * This class demonstrates various sliding window techniques commonly used in:
 * - Array processing
 * - String manipulation
 * - Stream processing
 * - Real-time data analysis
 */
class SlidingWindowAlgorithms
{
public:
   /**
    * @brief Find maximum element in each sliding window of size k
    * @param arr Input array
    * @param k Window size
    * @return Vector containing maximum element for each window position
    *
    * Algorithm: Uses monotonic decreasing deque
    * Time Complexity: O(n) - each element added and removed at most once
    * Space Complexity: O(k) - deque size limited by window size
    *
    * Key Insight: Maintain deque with indices in decreasing order of values.
    * The front always contains the index of maximum element in current window.
    */
   static vector<int> slidingWindowMaximum(const vector<int> &arr, int k)
   {
      if (arr.empty() || k <= 0 || k > arr.size())
      {
         return {};
      }

      vector<int> result;
      deque<int> indices; // Monotonic decreasing deque (stores indices)

      for (int i = 0; i < arr.size(); i++)
      {
         // Remove indices whose values are smaller than current element
         // This maintains the monotonic decreasing property
         while (!indices.empty() && arr[indices.back()] <= arr[i])
         {
            indices.pop_back();
         }

         // Add current index to the back
         indices.push_back(i);

         // Remove indices that are out of current window
         while (!indices.empty() && indices.front() <= i - k)
         {
            indices.pop_front();
         }

         // If we have processed at least k elements, record the maximum
         if (i >= k - 1)
         {
            result.push_back(arr[indices.front()]);
         }
      }

      return result;
   }

   /**
    * @brief Find minimum element in each sliding window of size k
    * @param arr Input array
    * @param k Window size
    * @return Vector containing minimum element for each window position
    *
    * Algorithm: Uses monotonic increasing deque (opposite of maximum)
    * Time Complexity: O(n)
    * Space Complexity: O(k)
    */
   static vector<int> slidingWindowMinimum(const vector<int> &arr, int k)
   {
      if (arr.empty() || k <= 0 || k > arr.size())
      {
         return {};
      }

      vector<int> result;
      deque<int> indices; // Monotonic increasing deque

      for (int i = 0; i < arr.size(); i++)
      {
         // Remove indices whose values are greater than current element
         while (!indices.empty() && arr[indices.back()] >= arr[i])
         {
            indices.pop_back();
         }

         indices.push_back(i);

         // Remove indices out of window
         while (!indices.empty() && indices.front() <= i - k)
         {
            indices.pop_front();
         }

         if (i >= k - 1)
         {
            result.push_back(arr[indices.front()]);
         }
      }

      return result;
   }

   /**
    * @brief Calculate sum of each sliding window of size k
    * @param arr Input array
    * @param k Window size
    * @return Vector containing sum for each window position
    *
    * Algorithm: Sliding window technique with running sum
    * Time Complexity: O(n)
    * Space Complexity: O(1)
    */
   static vector<long long> slidingWindowSum(const vector<int> &arr, int k)
   {
      if (arr.empty() || k <= 0 || k > arr.size())
      {
         return {};
      }

      vector<long long> result;
      long long windowSum = 0;

      // Calculate sum of first window
      for (int i = 0; i < k; i++)
      {
         windowSum += arr[i];
      }
      result.push_back(windowSum);

      // Slide the window: remove first element, add new element
      for (int i = k; i < arr.size(); i++)
      {
         windowSum = windowSum - arr[i - k] + arr[i];
         result.push_back(windowSum);
      }

      return result;
   }

   /**
    * @brief Find maximum sum of any subarray of size k
    * @param arr Input array
    * @param k Subarray size
    * @return Maximum sum found
    *
    * Time Complexity: O(n)
    * Space Complexity: O(1)
    */
   static long long maxSumSubarray(const vector<int> &arr, int k)
   {
      if (arr.empty() || k <= 0 || k > arr.size())
      {
         return 0;
      }

      long long maxSum = LLONG_MIN;
      long long windowSum = 0;

      // Calculate sum of first window
      for (int i = 0; i < k; i++)
      {
         windowSum += arr[i];
      }
      maxSum = windowSum;

      // Slide window and update maximum
      for (int i = k; i < arr.size(); i++)
      {
         windowSum = windowSum - arr[i - k] + arr[i];
         maxSum = max(maxSum, windowSum);
      }

      return maxSum;
   }

   /**
    * @brief Find length of longest substring with at most k distinct characters
    * @param s Input string
    * @param k Maximum distinct characters allowed
    * @return Length of longest valid substring
    *
    * Algorithm: Variable size sliding window with hash map
    * Time Complexity: O(n)
    * Space Complexity: O(k)
    */
   static int longestSubstringKDistinct(const string &s, int k)
   {
      if (s.empty() || k <= 0)
         return 0;

      unordered_map<char, int> charCount;
      int left = 0, maxLength = 0;

      for (int right = 0; right < s.length(); right++)
      {
         charCount[s[right]]++;

         // Shrink window if more than k distinct characters
         while (charCount.size() > k)
         {
            charCount[s[left]]--;
            if (charCount[s[left]] == 0)
            {
               charCount.erase(s[left]);
            }
            left++;
         }

         maxLength = max(maxLength, right - left + 1);
      }

      return maxLength;
   }

   /**
    * @brief Find length of longest substring without repeating characters
    * @param s Input string
    * @return Length of longest valid substring
    *
    * Algorithm: Variable size sliding window with set
    * Time Complexity: O(n)
    * Space Complexity: O(min(m,n)) where m is charset size
    */
   static int longestSubstringWithoutRepeating(const string &s)
   {
      if (s.empty())
         return 0;

      unordered_set<char> window;
      int left = 0, maxLength = 0;

      for (int right = 0; right < s.length(); right++)
      {
         // Shrink window until no duplicate
         while (window.count(s[right]))
         {
            window.erase(s[left]);
            left++;
         }

         window.insert(s[right]);
         maxLength = max(maxLength, right - left + 1);
      }

      return maxLength;
   }

   /**
    * @brief Find minimum window substring that contains all characters of pattern
    * @param s Source string
    * @param t Target pattern
    * @return Minimum window substring (empty if no valid window exists)
    *
    * Algorithm: Variable size sliding window with character frequency matching
    * Time Complexity: O(|s| + |t|)
    * Space Complexity: O(|t|)
    */
   static string minWindowSubstring(const string &s, const string &t)
   {
      if (s.empty() || t.empty() || s.length() < t.length())
      {
         return "";
      }

      unordered_map<char, int> targetCount, windowCount;
      for (char c : t)
      {
         targetCount[c]++;
      }

      int required = targetCount.size();
      int formed = 0;
      int left = 0, right = 0;
      int minLen = INT_MAX;
      int minLeft = 0;

      while (right < s.length())
      {
         // Expand window
         char c = s[right];
         windowCount[c]++;
         if (targetCount.count(c) && windowCount[c] == targetCount[c])
         {
            formed++;
         }

         // Contract window
         while (left <= right && formed == required)
         {
            c = s[left];

            // Update minimum window
            if (right - left + 1 < minLen)
            {
               minLen = right - left + 1;
               minLeft = left;
            }

            windowCount[c]--;
            if (targetCount.count(c) && windowCount[c] < targetCount[c])
            {
               formed--;
            }
            left++;
         }
         right++;
      }

      return minLen == INT_MAX ? "" : s.substr(minLeft, minLen);
   }
};

/**
 * @brief Legacy function for backward compatibility
 * Fixed version of the original sliding_max function
 */
void sliding_max(const vector<int> &arr, vector<int> &maxarr, int w)
{
   maxarr = SlidingWindowAlgorithms::slidingWindowMaximum(arr, w);
}

/**
 * @brief Comprehensive test suite for sliding window algorithms
 */
class SlidingWindowTests
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

public:
   static void runAllTests()
   {
      cout << "🎯 SLIDING WINDOW ALGORITHMS TEST SUITE" << endl;
      cout << "=======================================" << endl
           << endl;

      testSlidingWindowMaximum();
      testSlidingWindowMinimum();
      testSlidingWindowSum();
      testMaxSumSubarray();
      testLongestSubstringKDistinct();
      testLongestSubstringWithoutRepeating();
      testMinWindowSubstring();
      testEdgeCases();
      testLegacyFunction();

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
   static void testSlidingWindowMaximum()
   {
      cout << "📋 Testing Sliding Window Maximum" << endl;
      cout << "--------------------------------" << endl;

      // Test case 1: Basic functionality
      vector<int> arr1 = {1, 3, -1, -3, 5, 3, 6, 7};
      vector<int> expected1 = {3, 3, 5, 5, 6, 7};
      auto result1 = SlidingWindowAlgorithms::slidingWindowMaximum(arr1, 3);
      runTest("Basic sliding window maximum", result1 == expected1);

      // Test case 2: Window size 1
      vector<int> arr2 = {1, 2, 3, 4, 5};
      vector<int> expected2 = {1, 2, 3, 4, 5};
      auto result2 = SlidingWindowAlgorithms::slidingWindowMaximum(arr2, 1);
      runTest("Window size 1", result2 == expected2);

      // Test case 3: Decreasing array
      vector<int> arr3 = {5, 4, 3, 2, 1};
      vector<int> expected3 = {5, 4, 3};
      auto result3 = SlidingWindowAlgorithms::slidingWindowMaximum(arr3, 3);
      runTest("Decreasing array", result3 == expected3);

      // Test case 4: All same elements
      vector<int> arr4 = {2, 2, 2, 2, 2};
      vector<int> expected4 = {2, 2, 2};
      auto result4 = SlidingWindowAlgorithms::slidingWindowMaximum(arr4, 3);
      runTest("All same elements", result4 == expected4);

      cout << endl;
   }

   static void testSlidingWindowMinimum()
   {
      cout << "📋 Testing Sliding Window Minimum" << endl;
      cout << "--------------------------------" << endl;

      vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
      vector<int> expected = {-1, -3, -3, -3, 3, 3};
      auto result = SlidingWindowAlgorithms::slidingWindowMinimum(arr, 3);
      runTest("Basic sliding window minimum", result == expected);

      cout << endl;
   }

   static void testSlidingWindowSum()
   {
      cout << "📋 Testing Sliding Window Sum" << endl;
      cout << "----------------------------" << endl;

      vector<int> arr = {1, 2, 3, 4, 5, 6};
      vector<long long> expected = {6, 9, 12, 15};
      auto result = SlidingWindowAlgorithms::slidingWindowSum(arr, 3);
      runTest("Basic sliding window sum", result == expected);

      cout << endl;
   }

   static void testMaxSumSubarray()
   {
      cout << "📋 Testing Maximum Sum Subarray" << endl;
      cout << "------------------------------" << endl;

      vector<int> arr = {1, 4, 2, 10, 23, 3, 1, 0, 20};
      long long expected = 39; // [4, 2, 10, 23]
      auto result = SlidingWindowAlgorithms::maxSumSubarray(arr, 4);
      runTest("Maximum sum subarray", result == expected);

      cout << endl;
   }

   static void testLongestSubstringKDistinct()
   {
      cout << "📋 Testing Longest Substring K Distinct" << endl;
      cout << "--------------------------------------" << endl;

      string s = "eceba";
      int k = 2;
      int expected = 3; // "ece"
      auto result = SlidingWindowAlgorithms::longestSubstringKDistinct(s, k);
      runTest("Longest substring with k distinct chars", result == expected);

      cout << endl;
   }

   static void testLongestSubstringWithoutRepeating()
   {
      cout << "📋 Testing Longest Substring Without Repeating" << endl;
      cout << "---------------------------------------------" << endl;

      string s = "abcabcbb";
      int expected = 3; // "abc"
      auto result = SlidingWindowAlgorithms::longestSubstringWithoutRepeating(s);
      runTest("Longest substring without repeating", result == expected);

      cout << endl;
   }

   static void testMinWindowSubstring()
   {
      cout << "📋 Testing Minimum Window Substring" << endl;
      cout << "----------------------------------" << endl;

      string s = "ADOBECODEBANC";
      string t = "ABC";
      string expected = "BANC";
      auto result = SlidingWindowAlgorithms::minWindowSubstring(s, t);
      runTest("Minimum window substring", result == expected);

      cout << endl;
   }

   static void testEdgeCases()
   {
      cout << "📋 Testing Edge Cases" << endl;
      cout << "-------------------" << endl;

      // Empty array
      vector<int> empty;
      auto result1 = SlidingWindowAlgorithms::slidingWindowMaximum(empty, 3);
      runTest("Empty array", result1.empty());

      // Window size larger than array
      vector<int> small = {1, 2, 3};
      auto result2 = SlidingWindowAlgorithms::slidingWindowMaximum(small, 5);
      runTest("Window size > array size", result2.empty());

      // Single element
      vector<int> single = {42};
      vector<int> expected3 = {42};
      auto result3 = SlidingWindowAlgorithms::slidingWindowMaximum(single, 1);
      runTest("Single element array", result3 == expected3);

      cout << endl;
   }

   static void testLegacyFunction()
   {
      cout << "📋 Testing Legacy Function" << endl;
      cout << "-------------------------" << endl;

      vector<int> arr = {1, 3, -1, -3, 5, 3, 6, 7};
      vector<int> result;
      vector<int> expected = {3, 3, 5, 5, 6, 7};
      sliding_max(arr, result, 3);
      runTest("Legacy sliding_max function", result == expected);

      cout << endl;
   }
};

int SlidingWindowTests::testCount = 0;
int SlidingWindowTests::passedTests = 0;

/**
 * @brief Performance benchmark for sliding window algorithms
 */
class SlidingWindowBenchmark
{
public:
   static void runBenchmarks()
   {
      cout << "🚀 SLIDING WINDOW PERFORMANCE BENCHMARKS" << endl;
      cout << "=======================================" << endl
           << endl;

      vector<int> sizes = {1000, 10000, 100000, 500000};
      vector<int> windowSizes = {10, 100, 1000};

      for (int size : sizes)
      {
         cout << "📊 Array size: " << size << endl;
         cout << "------------------------" << endl;

         // Generate test data
         vector<int> testData = generateTestData(size);

         for (int windowSize : windowSizes)
         {
            if (windowSize <= size)
            {
               benchmarkAlgorithm("Sliding Window Maximum", testData, windowSize,
                                  [](const vector<int> &arr, int k)
                                  {
                                     return SlidingWindowAlgorithms::slidingWindowMaximum(arr, k).size();
                                  });
            }
         }
         cout << endl;
      }

      // String benchmarks
      cout << "📊 String Algorithm Benchmarks" << endl;
      cout << "-----------------------------" << endl;
      benchmarkStringAlgorithms();
   }

private:
   static vector<int> generateTestData(int size)
   {
      vector<int> data;
      data.reserve(size);
      for (int i = 0; i < size; i++)
      {
         data.push_back(rand() % 1000 - 500); // Random integers from -500 to 499
      }
      return data;
   }

   template <typename Func>
   static void benchmarkAlgorithm(const string &name, const vector<int> &data,
                                  int windowSize, Func algorithm)
   {
      auto start = high_resolution_clock::now();
      size_t result = algorithm(data, windowSize);
      auto end = high_resolution_clock::now();

      auto duration = duration_cast<microseconds>(end - start);

      cout << "  " << name << " (k=" << windowSize << "): "
           << duration.count() << "μs, "
           << result << " windows processed" << endl;
   }

   static void benchmarkStringAlgorithms()
   {
      string testString = generateTestString(100000);

      auto start = high_resolution_clock::now();
      int result1 = SlidingWindowAlgorithms::longestSubstringWithoutRepeating(testString);
      auto end = high_resolution_clock::now();
      auto duration1 = duration_cast<microseconds>(end - start);

      cout << "  Longest Substring Without Repeating: "
           << duration1.count() << "μs, result: " << result1 << endl;

      start = high_resolution_clock::now();
      int result2 = SlidingWindowAlgorithms::longestSubstringKDistinct(testString, 5);
      end = high_resolution_clock::now();
      auto duration2 = duration_cast<microseconds>(end - start);

      cout << "  Longest Substring K Distinct: "
           << duration2.count() << "μs, result: " << result2 << endl;

      cout << endl;
   }

   static string generateTestString(int length)
   {
      string result;
      result.reserve(length);
      for (int i = 0; i < length; i++)
      {
         result += 'a' + (rand() % 26);
      }
      return result;
   }
};

/**
 * @brief Interactive demonstration of sliding window algorithms
 */
class SlidingWindowDemo
{
public:
   static void runDemo()
   {
      cout << "🎮 SLIDING WINDOW INTERACTIVE DEMONSTRATION" << endl;
      cout << "===========================================" << endl
           << endl;

      demoSlidingWindowMaximum();
      demoVariableSizeWindow();
      demoRealWorldApplications();
   }

private:
   static void demoSlidingWindowMaximum()
   {
      cout << "🔍 Sliding Window Maximum Visualization" << endl;
      cout << "======================================" << endl;

      vector<int> arr = {4, 1, 3, 5, 2, 6, 1};
      int k = 3;

      cout << "Array: ";
      for (int x : arr)
         cout << x << " ";
      cout << endl;
      cout << "Window size: " << k << endl
           << endl;

      deque<int> indices;
      vector<int> result;

      for (int i = 0; i < arr.size(); i++)
      {
         cout << "Step " << (i + 1) << ": Processing element " << arr[i]
              << " at index " << i << endl;

         // Show current window
         cout << "  Current window: [";
         for (int j = max(0, i - k + 1); j <= i && j < arr.size(); j++)
         {
            cout << arr[j];
            if (j < i && j < arr.size() - 1)
               cout << ", ";
         }
         cout << "]" << endl;

         // Update deque
         while (!indices.empty() && arr[indices.back()] <= arr[i])
         {
            indices.pop_back();
         }
         indices.push_back(i);

         while (!indices.empty() && indices.front() <= i - k)
         {
            indices.pop_front();
         }

         if (i >= k - 1)
         {
            result.push_back(arr[indices.front()]);
            cout << "  🎯 Maximum in window: " << arr[indices.front()] << endl;
         }

         cout << endl;
      }

      cout << "Final result: ";
      for (int x : result)
         cout << x << " ";
      cout << endl
           << endl;
   }

   static void demoVariableSizeWindow()
   {
      cout << "🎯 Variable Size Window Demonstration" << endl;
      cout << "====================================" << endl;

      string s = "abcabcbb";
      cout << "Finding longest substring without repeating characters" << endl;
      cout << "String: \"" << s << "\"" << endl
           << endl;

      unordered_set<char> window;
      int left = 0, maxLen = 0;
      string maxSubstring;

      for (int right = 0; right < s.length(); right++)
      {
         cout << "Step " << (right + 1) << ": Adding '" << s[right] << "'" << endl;

         while (window.count(s[right]))
         {
            cout << "  Removing '" << s[left] << "' from left" << endl;
            window.erase(s[left]);
            left++;
         }

         window.insert(s[right]);

         cout << "  Current window: \"" << s.substr(left, right - left + 1)
              << "\" (length: " << (right - left + 1) << ")" << endl;

         if (right - left + 1 > maxLen)
         {
            maxLen = right - left + 1;
            maxSubstring = s.substr(left, maxLen);
            cout << "  🎯 New maximum length: " << maxLen << endl;
         }

         cout << endl;
      }

      cout << "Result: \"" << maxSubstring << "\" (length: " << maxLen << ")" << endl
           << endl;
   }

   static void demoRealWorldApplications()
   {
      cout << "🌍 Real-World Applications" << endl;
      cout << "=========================" << endl;

      cout << "1. 📊 Stock Price Analysis - Maximum price in sliding time windows" << endl;
      vector<int> stockPrices = {100, 102, 98, 105, 103, 107, 109, 104};
      cout << "   Stock prices: ";
      for (int price : stockPrices)
         cout << "$" << price << " ";
      cout << endl;

      auto maxPrices = SlidingWindowAlgorithms::slidingWindowMaximum(stockPrices, 3);
      cout << "   Max prices (3-day windows): ";
      for (int price : maxPrices)
         cout << "$" << price << " ";
      cout << endl
           << endl;

      cout << "2. 🌐 Network Traffic - Average bandwidth usage" << endl;
      vector<int> bandwidth = {50, 75, 60, 90, 85, 70, 95, 80};
      cout << "   Bandwidth usage (MB/s): ";
      for (int bw : bandwidth)
         cout << bw << " ";
      cout << endl;

      auto avgBandwidth = SlidingWindowAlgorithms::slidingWindowSum(bandwidth, 4);
      cout << "   4-minute averages: ";
      for (long long sum : avgBandwidth)
         cout << (sum / 4) << " ";
      cout << endl
           << endl;

      cout << "3. 🔐 Log Analysis - Finding suspicious activity patterns" << endl;
      string logData = "loginloginfailloginfailloginfailsuccess";
      cout << "   Log sequence: \"" << logData << "\"" << endl;
      int longestPattern = SlidingWindowAlgorithms::longestSubstringKDistinct(logData, 2);
      cout << "   Longest pattern with 2 distinct events: " << longestPattern << " chars" << endl
           << endl;
   }
};

/**
 * @brief Main function with comprehensive testing and demonstration
 */
int main(int argc, char *argv[])
{
   cout << "🎯 SLIDING WINDOW ALGORITHMS - COMPREHENSIVE IMPLEMENTATION" << endl;
   cout << "==========================================================" << endl;
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
      cout << "./sliding_window [options]" << endl
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
      SlidingWindowTests::runAllTests();
      cout << endl;
   }

   if (runDemo)
   {
      SlidingWindowDemo::runDemo();
      cout << endl;
   }

   if (runBenchmarks)
   {
      SlidingWindowBenchmark::runBenchmarks();
      cout << endl;
   }

   // Quick example if no specific options
   if (!runBenchmarks && !runDemo && runTests)
   {
      cout << "🎯 QUICK EXAMPLES" << endl;
      cout << "=================" << endl;

      vector<int> example = {1, 3, -1, -3, 5, 3, 6, 7};
      cout << "Input array: ";
      for (int x : example)
         cout << x << " ";
      cout << endl;

      auto maxResult = SlidingWindowAlgorithms::slidingWindowMaximum(example, 3);
      cout << "Sliding window maximum (k=3): ";
      for (int x : maxResult)
         cout << x << " ";
      cout << endl;

      auto minResult = SlidingWindowAlgorithms::slidingWindowMinimum(example, 3);
      cout << "Sliding window minimum (k=3): ";
      for (int x : minResult)
         cout << x << " ";
      cout << endl;

      auto sumResult = SlidingWindowAlgorithms::slidingWindowSum(example, 3);
      cout << "Sliding window sum (k=3): ";
      for (long long x : sumResult)
         cout << x << " ";
      cout << endl
           << endl;

      string strExample = "abcabcbb";
      cout << "String example: \"" << strExample << "\"" << endl;
      cout << "Longest substring without repeating: "
           << SlidingWindowAlgorithms::longestSubstringWithoutRepeating(strExample) << endl;
      cout << "Longest substring with 2 distinct chars: "
           << SlidingWindowAlgorithms::longestSubstringKDistinct(strExample, 2) << endl;
   }

   cout << "🎓 EDUCATIONAL NOTES" << endl;
   cout << "===================" << endl;
   cout << "• Sliding Window Maximum uses monotonic decreasing deque for O(n) time" << endl;
   cout << "• Variable size windows expand/contract based on conditions" << endl;
   cout << "• Two-pointer technique optimizes space complexity" << endl;
   cout << "• Applications: arrays, strings, streams, real-time data" << endl;
   cout << "• Key insight: Maintain invariants while sliding window" << endl
        << endl;

   cout << "🎊 Program completed successfully!" << endl;

   return 0;
}

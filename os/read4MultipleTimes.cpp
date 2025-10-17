/*
 * PROBLEM: Read N Characters Given Read4 II - Call Multiple Times
 *
 * Given a buffer that you can read 4 bytes at a time with a wrapper read4,
 * implement a read function that can be called multiple times while maintaining
 * state between calls.
 *
 * CONSTRAINTS:
 * - Can only use read4() API which reads up to 4 bytes at a time
 * - Must handle multiple calls to read() on the same file
 * - Must maintain state between calls (leftover bytes from previous reads)
 * - Must handle cases where n is not a multiple of 4
 * - Must handle end-of-file scenarios
 *
 * EXAMPLE:
 * Input buffer: "abc"
 * Multiple reads: [1, 2, 1]
 * Output: ["a", "bc", ""]
 *
 * The read4 API is defined in the parent class Reader4.
 *     int read4(char *buf4); // Returns number of bytes actually read (0-4)
 */

#include <algorithm> // For min() function
#include <cstring>   // For memcpy() function
#include <iostream>  // For testing
#include <string>    // For testing
#include <cassert>   // For testing
#include <vector>    // For testing

// Base class that provides read4() API
class Reader4
{
public:
    virtual int read4(char *buf4) = 0; // Pure virtual function
};

class Solution : public Reader4
{
private:
    // Global temporary buffer to store leftover bytes between read() calls
    char buf4[4];

    // Number of bytes left from previous read4() call that haven't been consumed
    int left = 0;

    // Length of the last read4() call (used to calculate starting index for leftovers)
    int last_len = 0;

public:
    /**
     * Reads n characters from file using read4 API with state persistence
     *
     * ALGORITHM:
     * 1. First, consume any leftover bytes from previous read() calls
     * 2. For remaining bytes needed:
     *    - If need >= 4 bytes: read directly into destination buffer
     *    - If need < 4 bytes: read into temp buffer and save leftovers
     * 3. Continue until n bytes read or EOF reached
     *
     * @param buf Destination buffer to store read characters
     * @param n   Number of characters to read
     * @return    The number of actual characters read (may be less than n if EOF)
     */
    int read(char *buf, int n)
    {
        int bytes_read = 0;        // Total bytes read so far in this call
        int len = 4;               // Bytes read from last read4() call (initialize to 4 for loop)
        int cur_bytes_to_copy = 0; // Bytes to copy in current iteration
        int index = 0;             // Starting index in buf4 for leftover bytes

        // STEP 1: Copy any leftover bytes from previous read() calls
        if (left != 0)
        {
            // Calculate how many leftover bytes we can use
            cur_bytes_to_copy = std::min(n, left);

            // Calculate starting index in buf4 where unconsumed bytes begin
            index = last_len - left;

            // Copy leftover bytes to destination buffer
            memcpy(buf, buf4 + index, cur_bytes_to_copy);

            // Update counters
            bytes_read += cur_bytes_to_copy;
            left -= cur_bytes_to_copy;
        }

        // STEP 2: Read additional bytes as needed
        // Continue while last read4() returned 4 bytes (not EOF) AND we need more bytes
        while (len == 4 && bytes_read < n)
        {
            if (n - bytes_read >= 4)
            {
                // CASE A: Buffer has space for 4+ bytes
                // Read directly into destination buffer (no temp storage needed)
                cur_bytes_to_copy = len = read4(buf + bytes_read);
            }
            else
            {
                // CASE B: Buffer has space for < 4 bytes
                // Read into temp buffer and save leftovers for next read() call
                last_len = len = read4(buf4);

                // Copy only what we need, save the rest
                cur_bytes_to_copy = std::min(len, n - bytes_read);
                memcpy(buf + bytes_read, buf4, cur_bytes_to_copy);

                // Calculate leftover bytes for next read() call
                left = len - cur_bytes_to_copy;
            }

            // Update total bytes read
            bytes_read += cur_bytes_to_copy;
        }

        return bytes_read;
    }

    // Dummy implementation for compilation (would be provided in actual implementation)
    int read4(char *buf4) override
    {
        // This would be implemented by the actual file reading system
        return 0;
    }

    // Reset state for testing (not part of original problem)
    void reset()
    {
        left = 0;
        last_len = 0;
        memset(buf4, 0, sizeof(buf4));
    }
};

// Mock implementation for testing
class TestSolution : public Reader4
{
private:
    std::string fileContent;
    int filePosition;

public:
    TestSolution(const std::string &content) : fileContent(content), filePosition(0) {}

    // Reset file position for new test
    void resetFile()
    {
        filePosition = 0;
    }

    // Mock read4 implementation that simulates reading from a file
    int read4(char *buf4) override
    {
        int bytesToRead = std::min(4, (int)(fileContent.length() - filePosition));

        if (bytesToRead <= 0)
        {
            return 0; // EOF
        }

        // Copy up to 4 bytes from fileContent to buf4
        for (int i = 0; i < bytesToRead; i++)
        {
            buf4[i] = fileContent[filePosition + i];
        }

        filePosition += bytesToRead;
        return bytesToRead;
    }

    /**
     * Same read implementation as Solution class for testing
     */
    int read(char *buf, int n)
    {
        int bytes_read = 0;
        int len = 4;
        int cur_bytes_to_copy = 0;
        int index = 0;

        // Copy any leftover bytes from previous read() calls
        if (left != 0)
        {
            cur_bytes_to_copy = std::min(n, left);
            index = last_len - left;
            memcpy(buf, buf4 + index, cur_bytes_to_copy);
            bytes_read += cur_bytes_to_copy;
            left -= cur_bytes_to_copy;
        }

        // Read additional bytes as needed
        while (len == 4 && bytes_read < n)
        {
            if (n - bytes_read >= 4)
            {
                cur_bytes_to_copy = len = read4(buf + bytes_read);
            }
            else
            {
                last_len = len = read4(buf4);
                cur_bytes_to_copy = std::min(len, n - bytes_read);
                memcpy(buf + bytes_read, buf4, cur_bytes_to_copy);
                left = len - cur_bytes_to_copy;
            }
            bytes_read += cur_bytes_to_copy;
        }

        return bytes_read;
    }

    // Reset state for testing
    void reset()
    {
        left = 0;
        last_len = 0;
        memset(buf4, 0, sizeof(buf4));
    }

private:
    char buf4[4];
    int left = 0;
    int last_len = 0;
};

// Test helper function for multiple reads
void runMultipleReadTest(const std::string &testName, const std::string &fileContent,
                         const std::vector<int> &readSizes,
                         const std::vector<std::string> &expectedResults)
{
    std::cout << "\n=== " << testName << " ===" << std::endl;
    std::cout << "File content: \"" << fileContent << "\"" << std::endl;
    std::cout << "Read sequence: [";
    for (size_t i = 0; i < readSizes.size(); i++)
    {
        std::cout << readSizes[i];
        if (i < readSizes.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]" << std::endl;

    TestSolution solution(fileContent);
    std::vector<std::string> actualResults;

    for (size_t i = 0; i < readSizes.size(); i++)
    {
        char buffer[1000] = {0};
        int bytesRead = solution.read(buffer, readSizes[i]);
        std::string result(buffer, bytesRead);
        actualResults.push_back(result);

        std::cout << "Read " << readSizes[i] << " bytes: \"" << result
                  << "\" (actual: " << bytesRead << " bytes)" << std::endl;
    }

    // Verify results
    assert(actualResults.size() == expectedResults.size());
    for (size_t i = 0; i < expectedResults.size(); i++)
    {
        assert(actualResults[i] == expectedResults[i]);
    }

    std::cout << "✅ PASSED" << std::endl;
}

// Single read test helper
void runSingleReadTest(const std::string &testName, const std::string &fileContent,
                       int n, const std::string &expectedResult, int expectedBytesRead)
{
    std::cout << "\n=== " << testName << " ===" << std::endl;
    std::cout << "File content: \"" << fileContent << "\"" << std::endl;
    std::cout << "Requested bytes: " << n << std::endl;

    TestSolution solution(fileContent);
    char buffer[1000] = {0};

    int actualBytesRead = solution.read(buffer, n);
    std::string actualResult(buffer, actualBytesRead);

    std::cout << "Expected: \"" << expectedResult << "\" (" << expectedBytesRead << " bytes)" << std::endl;
    std::cout << "Actual:   \"" << actualResult << "\" (" << actualBytesRead << " bytes)" << std::endl;

    assert(actualBytesRead == expectedBytesRead);
    assert(actualResult == expectedResult);

    std::cout << "✅ PASSED" << std::endl;
}

int main()
{
    std::cout << "🧪 Testing Read4 Multiple Times Implementation" << std::endl;
    std::cout << "=============================================" << std::endl;

    try
    {
        // Test 1: Example from problem description
        runMultipleReadTest("Test 1: Problem example [1,2,1] from 'abc'",
                            "abc", {1, 2, 1}, {"a", "bc", ""});

        // Test 2: Multiple small reads
        runMultipleReadTest("Test 2: Multiple small reads [1,1,1,1] from 'abcd'",
                            "abcd", {1, 1, 1, 1}, {"a", "b", "c", "d"});

        // Test 3: Large then small reads
        runMultipleReadTest("Test 3: Large then small [4,2,1,1] from 'abcdefgh'",
                            "abcdefgh", {4, 2, 1, 1}, {"abcd", "ef", "g", "h"});

        // Test 4: Small then large reads
        runMultipleReadTest("Test 4: Small then large [1,2,5] from 'abcdefgh'",
                            "abcdefgh", {1, 2, 5}, {"a", "bc", "defgh"});

        // Test 5: Reading past EOF
        runMultipleReadTest("Test 5: Reading past EOF [2,2,5] from 'abc'",
                            "abc", {2, 2, 5}, {"ab", "c", ""});

        // Test 6: Zero byte reads mixed with normal reads
        runMultipleReadTest("Test 6: Zero byte reads [0,2,0,1] from 'abc'",
                            "abc", {0, 2, 0, 1}, {"", "ab", "", "c"});

        // Test 7: Empty file multiple reads
        runMultipleReadTest("Test 7: Empty file [1,2,3] from ''",
                            "", {1, 2, 3}, {"", "", ""});

        // Test 8: Single character file multiple reads
        runMultipleReadTest("Test 8: Single char [1,1,1] from 'x'",
                            "x", {1, 1, 1}, {"x", "", ""});

        // Test 9: Large reads on small file
        runMultipleReadTest("Test 9: Large reads [10,5] from 'hello'",
                            "hello", {10, 5}, {"hello", ""});

        // Test 10: Exact boundary cases
        runMultipleReadTest("Test 10: Boundary [4,4] from 'abcdefgh'",
                            "abcdefgh", {4, 4}, {"abcd", "efgh"});

        // Additional single read tests for completeness
        runSingleReadTest("Single Test 1: Normal read", "hello world", 5, "hello", 5);
        runSingleReadTest("Single Test 2: Read beyond EOF", "hi", 10, "hi", 2);
        runSingleReadTest("Single Test 3: Zero bytes", "test", 0, "", 0);

        std::cout << "\n🎉 All tests passed successfully!" << std::endl;
        std::cout << "\n📋 Test Summary:" << std::endl;
        std::cout << "   • Multiple read calls: ✅" << std::endl;
        std::cout << "   • State persistence: ✅" << std::endl;
        std::cout << "   • Leftover byte handling: ✅" << std::endl;
        std::cout << "   • EOF scenarios: ✅" << std::endl;
        std::cout << "   • Boundary conditions: ✅" << std::endl;
        std::cout << "   • Edge cases: ✅" << std::endl;

        std::cout << "\n🔧 To compile and run:" << std::endl;
        std::cout << "   g++ -std=c++11 -o read4_multiple_test read4MultipleTimes.cpp && ./read4_multiple_test" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

/*
 * 📚 ALGORITHM EXPLANATION FOR MULTIPLE READS:
 *
 * The key difference from single read4 is STATE MANAGEMENT between calls:
 *
 * 1. GLOBAL STATE VARIABLES:
 *    - buf4[4]: Temporary buffer that persists between read() calls
 *    - left: Number of unconsumed bytes in buf4 from previous read()
 *    - last_len: Length of last read4() call (to calculate start index)
 *
 * 2. ALGORITHM FLOW:
 *    a) CONSUME LEFTOVERS: Use any remaining bytes from previous calls
 *    b) DIRECT READ: If need ≥4 bytes, read directly to destination
 *    c) BUFFERED READ: If need <4 bytes, read to temp buffer, save excess
 *
 * 3. STATE PERSISTENCE EXAMPLE:
 *    File: "abcdefgh", Reads: [3, 2, 3]
 *
 *    Read 1 (n=3):
 *    - read4() → "abcd" into buf4
 *    - Copy "abc" to result, left=1 ("d" remains)
 *    - Returns "abc"
 *
 *    Read 2 (n=2):
 *    - Use leftover "d", need 1 more
 *    - read4() → "efgh" into buf4
 *    - Copy "d" + "e" to result, left=3 ("fgh" remains)
 *    - Returns "de"
 *
 *    Read 3 (n=3):
 *    - Use leftover "fgh"
 *    - Returns "fgh"
 *
 * 🔍 KEY INSIGHTS:
 * - Maintains state between multiple read() calls on same file
 * - Efficiently handles partial consumption of read4() results
 * - Minimizes read4() calls by reusing buffered data
 * - Handles arbitrary read sizes with optimal performance
 *
 * ⏱️ COMPLEXITY:
 * - Time: O(n) amortized over multiple calls
 * - Space: O(1) using fixed-size buffer
 */

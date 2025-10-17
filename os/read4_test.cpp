/*
 * TEST FILE for Read4 Implementation
 * This file contains a complete testable version of the read4 solution
 */

#include <algorithm> // For min() function
#include <cstring>   // For memcpy() function
#include <iostream>  // For testing
#include <string>    // For testing
#include <cassert>   // For testing

// Base class that provides read4() API
class Reader4
{
public:
    virtual int read4(char *buf4) = 0; // Pure virtual function
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
    void reset()
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
     * Reads n characters from file using read4 API
     *
     * ALGORITHM:
     * 1. Use temporary buffer to read 4 bytes at a time
     * 2. Copy only needed bytes to destination buffer
     * 3. Continue until we've read n bytes or hit end of file
     *
     * @param buf Destination buffer to store read characters
     * @param n   Number of characters to read
     * @return    The number of actual characters read (may be less than n if EOF)
     */
    int read(char *buf, int n)
    {
        // Track total bytes read so far
        int read_bytes = 0;

        // Initialize len to 4 to ensure loop runs at least once
        // This will be updated with actual bytes read from read4()
        int len = 4;

        // File pointer (not actively used in this implementation)
        int fp = 0;

        // Temporary buffer to store 4 bytes from each read4() call
        // This prevents buffer overflow in the destination buffer
        char buf4[4];

        // Number of bytes to copy from temp buffer to destination in current iteration
        int cur_bytes_to_copy = 0;

        // Continue reading while:
        // 1. We haven't read n bytes yet (read_bytes < n)
        // 2. Last read4() call read exactly 4 bytes (len == 4)
        //    If len < 4, we've hit end of file
        while (read_bytes < n && len == 4)
        {
            // Read up to 4 bytes into temporary buffer
            // len will contain actual number of bytes read (0-4)
            len = read4(buf4);

            // Calculate how many bytes to copy:
            // - Don't copy more than what was actually read (len)
            // - Don't copy more than what's still needed (n - read_bytes)
            cur_bytes_to_copy = std::min(len, n - read_bytes);

            // Copy the calculated bytes from temp buffer to destination
            // buf + read_bytes: destination position (advance by bytes already read)
            // buf4: source (temporary buffer)
            // cur_bytes_to_copy: number of bytes to copy
            memcpy(buf + read_bytes, buf4, cur_bytes_to_copy);

            // Update total bytes read counter
            read_bytes += cur_bytes_to_copy;
        }

        // Return actual number of bytes read
        // This may be less than n if we hit end of file
        return read_bytes;
    }
};

// Test helper function
void runTest(const std::string &testName, const std::string &fileContent, int n,
             const std::string &expectedResult, int expectedBytesRead)
{
    std::cout << "\n=== " << testName << " ===" << std::endl;
    std::cout << "File content: \"" << fileContent << "\"" << std::endl;
    std::cout << "Requested bytes: " << n << std::endl;

    TestSolution solution(fileContent);
    char buffer[1000] = {0}; // Initialize buffer with zeros

    int actualBytesRead = solution.read(buffer, n);
    std::string actualResult(buffer, actualBytesRead);

    std::cout << "Expected: \"" << expectedResult << "\" (" << expectedBytesRead << " bytes)" << std::endl;
    std::cout << "Actual:   \"" << actualResult << "\" (" << actualBytesRead << " bytes)" << std::endl;

    // Verify results
    assert(actualBytesRead == expectedBytesRead);
    assert(actualResult == expectedResult);

    std::cout << "✅ PASSED" << std::endl;
}

// Test edge cases
void runEdgeCaseTest(const std::string &testName, const std::string &fileContent, int n)
{
    std::cout << "\n=== " << testName << " ===" << std::endl;
    std::cout << "File content: \"" << fileContent << "\" (length: " << fileContent.length() << ")" << std::endl;
    std::cout << "Requested bytes: " << n << std::endl;

    TestSolution solution(fileContent);
    char buffer[1000] = {0};

    int actualBytesRead = solution.read(buffer, n);
    std::string actualResult(buffer, actualBytesRead);

    std::cout << "Read: \"" << actualResult << "\" (" << actualBytesRead << " bytes)" << std::endl;

    // For edge cases, just verify basic constraints
    assert(actualBytesRead >= 0);
    assert(actualBytesRead <= n);
    assert(actualBytesRead <= (int)fileContent.length());

    std::cout << "✅ PASSED (Edge case validated)" << std::endl;
}

int main()
{
    std::cout << "🧪 Testing Read4 Implementation" << std::endl;
    std::cout << "===============================" << std::endl;

    try
    {
        // Test 1: Reading exactly n bytes (n < file size, n is multiple of 4)
        runTest("Test 1: Read 8 bytes from 12-byte file",
                "Hello World!", 8, "Hello Wo", 8);

        // Test 2: Reading n bytes where n is not multiple of 4
        runTest("Test 2: Read 7 bytes from long file",
                "Hello World!", 7, "Hello W", 7);

        // Test 3: Reading more bytes than available (EOF case)
        runTest("Test 3: Read 10 bytes from 7-byte file",
                "Hello!!", 10, "Hello!!", 7);

        // Test 4: Reading fewer bytes than one read4 call
        runTest("Test 4: Read 3 bytes from long file",
                "Hello World!", 3, "Hel", 3);

        // Test 5: Reading from empty file
        runTest("Test 5: Read from empty file",
                "", 5, "", 0);

        // Test 6: Reading 0 bytes
        runTest("Test 6: Read 0 bytes",
                "Hello", 0, "", 0);

        // Test 7: Reading exact file size
        runTest("Test 7: Read exact file size",
                "Test", 4, "Test", 4);

        // Test 8: Large read request
        runTest("Test 8: Read large amount",
                "Short", 100, "Short", 5);

        // Test 9: Single character file
        runTest("Test 9: Single character file",
                "A", 1, "A", 1);

        // Test 10: Multiple read4 calls needed
        runTest("Test 10: Multiple read4 calls",
                "This is a longer string for testing", 15, "This is a longe", 15);

        // Additional edge case tests
        runEdgeCaseTest("Edge Test 1: Read 1 byte from 1-byte file", "X", 1);
        runEdgeCaseTest("Edge Test 2: Read exactly 4 bytes", "1234", 4);
        runEdgeCaseTest("Edge Test 3: Read 5 bytes from 4-byte file", "abcd", 5);
        runEdgeCaseTest("Edge Test 4: Large file, small read",
                        "This is a very long string that spans multiple read4 calls", 6);

        std::cout << "\n🎉 All tests passed successfully!" << std::endl;
        std::cout << "\n📋 Test Summary:" << std::endl;
        std::cout << "   • Basic functionality: ✅" << std::endl;
        std::cout << "   • Edge cases: ✅" << std::endl;
        std::cout << "   • EOF handling: ✅" << std::endl;
        std::cout << "   • Buffer overflow protection: ✅" << std::endl;
        std::cout << "   • Partial reads: ✅" << std::endl;

        std::cout << "\n🔧 To compile and run:" << std::endl;
        std::cout << "   g++ -std=c++11 -o read4_test read4_test.cpp && ./read4_test" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

/*
 * 📚 ALGORITHM EXPLANATION:
 *
 * The read() function implements a buffered reading strategy:
 *
 * 1. INITIALIZATION:
 *    - read_bytes: Tracks total bytes read so far
 *    - len: Set to 4 initially to enter the loop
 *    - buf4[4]: Temporary buffer for read4() calls
 *    - cur_bytes_to_copy: Calculated bytes to copy each iteration
 *
 * 2. MAIN LOOP (while read_bytes < n && len == 4):
 *    - Condition ensures we stop when:
 *      a) We've read enough bytes (read_bytes >= n), OR
 *      b) We hit EOF (len < 4 from last read4())
 *
 * 3. EACH ITERATION:
 *    - Call read4() to get up to 4 bytes
 *    - Calculate how many to copy: min(actual_read, remaining_needed)
 *    - Copy calculated bytes to destination buffer
 *    - Update total bytes read counter
 *
 * 4. RETURN:
 *    - Actual bytes read (may be less than requested if EOF)
 *
 * 🔍 KEY INSIGHTS:
 * - Uses temporary buffer to prevent destination buffer overflow
 * - Handles partial reads gracefully (when n is not multiple of 4)
 * - Stops reading at EOF or when n bytes are reached
 * - Memory safe: never writes beyond destination buffer bounds
 *
 * ⏱️ COMPLEXITY:
 * - Time: O(n) where n is bytes to read
 * - Space: O(1) using fixed-size temporary buffer
 */
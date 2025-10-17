/*
 * PROBLEM: Read N Characters Given Read4
 *
 * Given a buffer that you can read 4 bytes at a time with a wrapper
 * read4, need to read n bytes into a buffer.
 *
 * CONSTRAINTS:
 * - Can only use read4() API which reads up to 4 bytes at a time
 * - Must handle cases where n is not a multiple of 4
 * - Must handle end-of-file scenarios
 * - Must not read more than n bytes into destination buffer
 *
 * The read4 API is defined in the parent class Reader4.
 *     int read4(char *buf4); // Returns number of bytes actually read (0-4)
 */

#include <algorithm> // For min() function
#include <cstring>   // For memcpy() function

// Base class that provides read4() API (would be provided in actual implementation)
class Reader4
{
public:
    virtual int read4(char *buf4) = 0; // Pure virtual function
};

class Solution : public Reader4
{
public:
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

    // Dummy implementation for compilation (would be provided in actual implementation)
    int read4(char *buf4) override
    {
        // This would be implemented by the actual file reading system
        return 0;
    }
};

/*
 * EXAMPLE SCENARIOS:
 *
 * 1. Reading 7 bytes from file with content "Hello World":
 *    - First read4(): reads "Hell" (4 bytes), copies all 4
 *    - Second read4(): reads "o Wo" (4 bytes), copies only 3 ("o W")
 *    - Total: 7 bytes read
 *
 * 2. Reading 10 bytes from 8-byte file "Hello!!":
 *    - First read4(): reads "Hell" (4 bytes), copies all 4
 *    - Second read4(): reads "o!!" + EOF (3 bytes), copies all 3
 *    - Third read4(): reads 0 bytes (EOF), loop exits
 *    - Total: 7 bytes read (less than requested 10)
 *
 * 3. Reading 3 bytes from file:
 *    - First read4(): reads 4 bytes, copies only 3
 *    - Total: 3 bytes read, loop exits
 *
 * TIME COMPLEXITY: O(n) - where n is the number of bytes to read
 * SPACE COMPLEXITY: O(1) - only using fixed-size temporary buffer
 */

/*
 * EXAMPLE SCENARIOS:
 *
 * 1. Reading 7 bytes from file with content "Hello World":
 *    - First read4(): reads "Hell" (4 bytes), copies all 4
 *    - Second read4(): reads "o Wo" (4 bytes), copies only 3 ("o W")
 *    - Total: 7 bytes read
 *
 * 2. Reading 10 bytes from 8-byte file "Hello!!":
 *    - First read4(): reads "Hell" (4 bytes), copies all 4
 *    - Second read4(): reads "o!!" + EOF (3 bytes), copies all 3
 *    - Third read4(): reads 0 bytes (EOF), loop exits
 *    - Total: 7 bytes read (less than requested 10)
 *
 * 3. Reading 3 bytes from file:
 *    - First read4(): reads 4 bytes, copies only 3
 *    - Total: 3 bytes read, loop exits
 *
 * TIME COMPLEXITY: O(n) - where n is the number of bytes to read
 * SPACE COMPLEXITY: O(1) - only using fixed-size temporary buffer
 */

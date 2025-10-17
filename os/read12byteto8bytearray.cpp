
/*
 * PROBLEM: Convert 12-bit Values to 8-bit Array
 *
 * Hardware (microcontroller) feeds 12-bit values represented as 4-byte integers
 * with only the least significant 12 bits being valid. Convert these values
 * into an array of 8-bit unsigned values by packing them efficiently.
 *
 * EXAMPLE:
 * Input:  [0x123, 0x456] (12-bit values: 0001 0010 0011, 0100 0101 0110)
 * Output: [0x12, 0x34, 0x56] (8-bit values: 0001 0010, 0011 0100, 0101 0110)
 *
 * ALGORITHM:
 * - Pack 12-bit values into bytes by grouping bits efficiently
 * - Every 2 input values (24 bits) produce 3 output bytes (24 bits)
 * - Handle odd number of inputs by padding the final byte
 *
 * CONSTRAINTS:
 * - Input: 32-bit integers with only lower 12 bits valid
 * - Output: Array of 8-bit unsigned integers
 * - Must preserve all 12-bit data without loss
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include <cassert>
#include <string>

using namespace std;

// Mask to extract valid 12 bits from 32-bit input
#define VALID_BITS_MASK 0xFFF

/**
 * Converts array of 12-bit values (stored in 32-bit integers) to 8-bit array
 *
 * DETAILED ALGORITHM:
 * The key insight is that we need to pack 12-bit values efficiently:
 * - 2 input values (2 × 12 = 24 bits) → 3 output bytes (3 × 8 = 24 bits)
 *
 * For inputs A and B (each 12 bits):
 * A = [A11 A10 A9 A8 A7 A6 A5 A4 A3 A2 A1 A0]
 * B = [B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0]
 *
 * Output bytes:
 * Byte1: [A11 A10 A9 A8 A7 A6 A5 A4] = A >> 4
 * Byte2: [A3 A2 A1 A0 B11 B10 B9 B8] = (A & 0xF) << 4 | (B >> 8)
 * Byte3: [B7 B6 B5 B4 B3 B2 B1 B0] = B & 0xFF
 *
 * @param array Pointer to array of 32-bit integers with 12-bit valid data
 * @param len   Length of input array
 * @return      Vector of 8-bit unsigned integers
 */
vector<uint8_t> convert12BitTo8BitArray(uint32_t *array, int len)
{
	vector<uint8_t> result;

	// Handle empty input
	if (len <= 0 || array == nullptr)
	{
		return result;
	}

	int i = 0;
	uint32_t currentValue = 0;
	uint32_t firstByte = 0;
	uint32_t secondByte = 0;
	bool hasRemainder = false; // Tracks if we have leftover 4 bits from previous iteration

	while (i < len)
	{
		// Extract valid 12 bits from current 32-bit input
		currentValue = array[i] & VALID_BITS_MASK;

		if (hasRemainder)
		{
			// CASE: We have 4 leftover bits from previous iteration
			// Combine leftover 4 bits with upper 4 bits of current value
			firstByte = (secondByte << 4) | ((currentValue & 0xF00) >> 8);

			// Lower 8 bits of current value become next byte
			secondByte = currentValue & 0xFF;

			hasRemainder = false; // We've consumed the remainder

			// Add the combined byte
			result.push_back(firstByte);
			// Add the lower 8 bits
			result.push_back(secondByte);
		}
		else
		{
			// CASE: No remainder from previous iteration
			// Split current 12-bit value: upper 8 bits + lower 4 bits
			firstByte = (currentValue & VALID_BITS_MASK) >> 4; // Upper 8 bits
			secondByte = currentValue & 0xF;				   // Lower 4 bits

			hasRemainder = true; // We have 4 bits left over

			// Add the upper 8 bits immediately
			result.push_back(firstByte);
			// Keep lower 4 bits for next iteration (stored in secondByte)
		}

		i++;
	}

	// Handle final remainder if odd number of inputs
	if (hasRemainder)
	{
		// Left-shift the remaining 4 bits to make a complete byte
		// Pad with zeros in the lower 4 bits
		result.push_back(secondByte << 4);
	}

	return result;
}

/**
 * Helper function to print array in hexadecimal format
 */
void printHexArray(const vector<uint8_t> &arr, const string &label)
{
	cout << label << ": [";
	for (size_t i = 0; i < arr.size(); i++)
	{
		cout << "0x" << hex << uppercase << setfill('0') << setw(2)
			 << static_cast<unsigned int>(arr[i]);
		if (i < arr.size() - 1)
			cout << ", ";
	}
	cout << "]" << dec << endl;
}

/**
 * Helper function to print 32-bit array showing only valid 12 bits
 */
void print12BitArray(uint32_t *arr, int len, const string &label)
{
	cout << label << ": [";
	for (int i = 0; i < len; i++)
	{
		cout << "0x" << hex << uppercase << (arr[i] & VALID_BITS_MASK);
		if (i < len - 1)
			cout << ", ";
	}
	cout << "]" << dec << endl;
}

/**
 * Test helper function
 */
void runTest(const string &testName, uint32_t *input, int inputLen,
			 const vector<uint8_t> &expected)
{
	cout << "\n=== " << testName << " ===" << endl;

	print12BitArray(input, inputLen, "Input (12-bit values)");

	vector<uint8_t> result = convert12BitTo8BitArray(input, inputLen);

	printHexArray(expected, "Expected");
	printHexArray(result, "Actual  ");

	// Verify results
	assert(result.size() == expected.size());
	for (size_t i = 0; i < expected.size(); i++)
	{
		assert(result[i] == expected[i]);
	}

	cout << "✅ PASSED" << endl;
}

int main()
{
	cout << "🧪 Testing 12-bit to 8-bit Array Conversion" << endl;
	cout << "===========================================" << endl;

	try
	{
		// Test 1: Basic example from problem description
		uint32_t test1[] = {0x123, 0x456};
		runTest("Test 1: Basic conversion [0x123, 0x456]",
				test1, 2, {0x12, 0x34, 0x56});

		// Test 2: Three values (odd number)
		uint32_t test2[] = {0x123, 0x456, 0x789};
		runTest("Test 2: Odd number of inputs [0x123, 0x456, 0x789]",
				test2, 3, {0x12, 0x34, 0x56, 0x78, 0x90});

		// Test 3: Single value
		uint32_t test3[] = {0xABC};
		runTest("Test 3: Single value [0xABC]",
				test3, 1, {0xAB, 0xC0});

		// Test 4: Maximum 12-bit values
		uint32_t test4[] = {0xFFF, 0xFFF};
		runTest("Test 4: Maximum values [0xFFF, 0xFFF]",
				test4, 2, {0xFF, 0xFF, 0xFF});

		// Test 5: Minimum values (zeros)
		uint32_t test5[] = {0x000, 0x000};
		runTest("Test 5: Zero values [0x000, 0x000]",
				test5, 2, {0x00, 0x00, 0x00});

		// Test 6: Mixed values with extra bits in 32-bit input (should be masked)
		uint32_t test6[] = {0x12345123, 0x98765456}; // Extra bits should be ignored
		runTest("Test 6: Values with extra bits [0x12345123, 0x98765456] → [0x123, 0x456]",
				test6, 2, {0x12, 0x34, 0x56});

		// Test 7: Four values (even number > 2)
		uint32_t test7[] = {0x111, 0x222, 0x333, 0x444};
		runTest("Test 7: Four values [0x111, 0x222, 0x333, 0x444]",
				test7, 4, {0x11, 0x12, 0x22, 0x33, 0x34, 0x44});

		// Test 8: Empty array
		uint32_t *test8 = nullptr;
		runTest("Test 8: Empty array",
				test8, 0, {});

		// Test 9: Boundary values
		uint32_t test9[] = {0x001, 0xFFE};
		runTest("Test 9: Boundary values [0x001, 0xFFE]",
				test9, 2, {0x00, 0x1F, 0xFE});

		// Test 10: Pattern test
		uint32_t test10[] = {0xAAA, 0x555, 0xAAA};
		runTest("Test 10: Pattern test [0xAAA, 0x555, 0xAAA]",
				test10, 3, {0xAA, 0xA5, 0x55, 0xAA, 0xA0});

		cout << "\n🎉 All tests passed successfully!" << endl;
		cout << "\n📋 Algorithm Summary:" << endl;
		cout << "   • Packs 12-bit values into 8-bit array efficiently" << endl;
		cout << "   • 2 input values (24 bits) → 3 output bytes (24 bits)" << endl;
		cout << "   • Handles odd inputs by padding final byte" << endl;
		cout << "   • Masks extra bits from 32-bit input values" << endl;
		cout << "   • Zero data loss during conversion" << endl;

		cout << "\n🔧 To compile and run:" << endl;
		cout << "   g++ -std=c++11 -o convert12to8_test read12byteto8bytearray.cpp && ./convert12to8_test" << endl;
	}
	catch (const exception &e)
	{
		cout << "❌ Test failed with exception: " << e.what() << endl;
		return 1;
	}

	return 0;
}

/*
 * 📚 DETAILED ALGORITHM EXPLANATION:
 *
 * PROBLEM CONTEXT:
 * - Microcontroller provides 12-bit data in 32-bit containers
 * - Need to pack this efficiently into 8-bit array
 * - Must preserve all data without loss
 *
 * BIT-LEVEL PACKING STRATEGY:
 *
 * Input:  Two 12-bit values A and B
 * A = [A11 A10 A9 A8 | A7 A6 A5 A4 | A3 A2 A1 A0]
 * B = [B11 B10 B9 B8 | B7 B6 B5 B4 | B3 B2 B1 B0]
 *
 * Output: Three 8-bit bytes
 * Byte1 = [A11 A10 A9 A8 A7 A6 A5 A4] = A >> 4
 * Byte2 = [A3 A2 A1 A0 B11 B10 B9 B8] = (A & 0xF) << 4 | (B >> 8)
 * Byte3 = [B7 B6 B5 B4 B3 B2 B1 B0] = B & 0xFF
 *
 * EXAMPLE WALKTHROUGH:
 * Input: [0x123, 0x456]
 *
 * 0x123 = 0001 0010 0011 (binary)
 * 0x456 = 0100 0101 0110 (binary)
 *
 * Step 1: A = 0x123
 * - firstByte = 0x123 >> 4 = 0x12
 * - remainder = 0x123 & 0xF = 0x3
 * - Output: [0x12], remainder = 0x3
 *
 * Step 2: B = 0x456, has remainder 0x3
 * - firstByte = (0x3 << 4) | (0x456 >> 8) = 0x30 | 0x4 = 0x34
 * - secondByte = 0x456 & 0xFF = 0x56
 * - Output: [0x12, 0x34, 0x56], remainder = 0
 *
 * EFFICIENCY:
 * - Time Complexity: O(n) where n is input array length
 * - Space Complexity: O(n) for output array
 * - Optimal bit packing: 24 input bits → 24 output bits (no waste)
 * - Handles arbitrary input lengths gracefully
 */

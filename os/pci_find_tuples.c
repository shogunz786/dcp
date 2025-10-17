/*
 * PROBLEM: PCI Configuration ROM Tuple Finder
 *
 * Parse PCI Configuration ROM data to find specific configuration tuples.
 * PCI Configuration ROM contains structured data organized as tuples,
 * where each tuple has an ID, a pointer to the next tuple, data length,
 * and the actual data payload.
 *
 * TUPLE STRUCTURE:
 * Each tuple in the ROM has the following layout:
 * [  ID  |  NEXT  |  LEN  |    DATA...    ]
 * [uint16|uint16  |uint16 | variable size ]
 *
 * - ID: Unique identifier for the tuple type
 * - NEXT: Offset from ROM start to next tuple (0xFFFF = end)
 * - LEN: Length of data portion in bytes
 * - DATA: Variable-length payload data
 *
 * ALGORITHM:
 * 1. Start at beginning of ROM
 * 2. Parse tuple header (ID, NEXT, LEN)
 * 3. If ID matches target, return pointer to data
 * 4. If not, follow NEXT pointer to next tuple
 * 5. Continue until match found or end marker (0xFFFF)
 *
 * SAFETY FEATURES:
 * - Bounds checking to prevent infinite loops
 * - Null pointer validation
 * - End-of-chain detection with 0xFFFF marker
 * - Maximum iteration limit for safety
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

// PCI ROM constants
#define PCI_ROM_MAX_SIZE (64 * 1024)				 // Maximum ROM size (64KB)
#define PCI_TUPLE_END 0xFFFF						 // End of tuple chain marker
#define PCI_TUPLE_HEADER_SIZE (3 * sizeof(uint16_t)) // ID + NEXT + LEN

// Maximum number of tuples to prevent infinite loops
#define MAX_TUPLES_TO_SCAN (PCI_ROM_MAX_SIZE / PCI_TUPLE_HEADER_SIZE)

/**
 * Configuration Data Tuple Structure
 *
 * Represents a single configuration tuple in PCI ROM
 * Layout in memory:
 * [   ID   ] - 2 bytes - Tuple identifier
 * [ NEXT   ] - 2 bytes - Offset to next tuple (from ROM start)
 * [  LEN   ] - 2 bytes - Length of data portion
 * [ DATA.. ] - Variable - Actual configuration data
 */
typedef struct
{
	uint16_t id;	   // Tuple identifier
	uint16_t ptr_next; // Offset from start of ROM to next tuple
	uint16_t data_len; // Length of data portion in bytes
	uint8_t *data;	   // Pointer to data (not stored in ROM, calculated)
} config_data_t;

/**
 * Find a specific tuple in PCI Configuration ROM
 *
 * DETAILED ALGORITHM:
 * 1. Start at ROM beginning, cast to byte pointer for address arithmetic
 * 2. Parse current tuple header: extract ID, NEXT offset, and data length
 * 3. Compare current tuple ID with target ID
 * 4. If match found: calculate data pointer and return it
 * 5. If no match: follow NEXT pointer to continue search
 * 6. Check for end conditions: 0xFFFF marker or maximum iterations
 * 7. Return NULL if tuple not found
 *
 * MEMORY LAYOUT EXAMPLE:
 * ROM Address: [0x1000] [0x1002] [0x1004] [0x1006] [0x1008] [0x100A] ...
 * Content:     [  ID1 ] [ NEXT1] [ LEN1 ] [DATA1_0] [DATA1_1] [  ID2 ] ...
 *
 * @param rom Pointer to the start of PCI Configuration ROM
 * @param id  Target tuple ID to search for
 * @param len Pointer to store the length of found tuple data (output)
 * @return    Pointer to tuple data if found, NULL otherwise
 */
uint16_t *find_tuple(const uint16_t *rom, uint16_t id, size_t *len)
{
	// Input validation
	if (rom == NULL || len == NULL)
	{
		return NULL;
	}

	// Initialize search variables
	uint8_t *current_offset = (uint8_t *)rom; // Current position in ROM
	uint16_t *data_ptr = NULL;				  // Pointer to tuple data
	uint16_t current_id = 0;				  // ID of current tuple
	uint16_t next_offset = 0;				  // Offset to next tuple
	uint16_t data_length = 0;				  // Length of current tuple data
	int tuples_scanned = 0;					  // Safety counter

	// Search loop: continue until tuple found or end reached
	while (tuples_scanned < MAX_TUPLES_TO_SCAN)
	{
		// Parse tuple header at current position
		// Layout: [ID][NEXT][LEN][DATA...]

		// Extract tuple ID (first 2 bytes)
		current_id = *(uint16_t *)current_offset;

		// Check if this is the tuple we're looking for
		if (current_id == id)
		{
			// Found target tuple - extract data information

			// Get data length (third 2-byte field)
			data_length = *((uint16_t *)(current_offset + 2 * sizeof(uint16_t)));

			// Calculate pointer to data portion (after 3-field header)
			data_ptr = (uint16_t *)(current_offset + PCI_TUPLE_HEADER_SIZE);

			// Return data length to caller
			*len = data_length;

			return data_ptr;
		}

		// Current tuple doesn't match - move to next tuple

		// Extract next tuple offset (second 2-byte field)
		next_offset = *(uint16_t *)(current_offset + sizeof(uint16_t));

		// Check for end of tuple chain
		if (next_offset == PCI_TUPLE_END)
		{
			break; // No more tuples to search
		}

		// Bounds checking: ensure offset is within ROM
		if (next_offset >= PCI_ROM_MAX_SIZE)
		{
			break; // Invalid offset, prevent buffer overflow
		}

		// Move to next tuple
		current_offset = ((uint8_t *)rom) + next_offset;
		tuples_scanned++;
	}

	// Tuple not found
	return NULL;
}

/**
 * Validate tuple structure and data integrity
 *
 * @param rom Pointer to ROM data
 * @param rom_size Size of ROM data
 * @return true if ROM structure is valid, false otherwise
 */
bool validate_rom_structure(const uint16_t *rom, size_t rom_size)
{
	if (rom == NULL || rom_size == 0)
	{
		return false;
	}

	uint8_t *current_offset = (uint8_t *)rom;
	int tuples_found = 0;

	while (tuples_found < MAX_TUPLES_TO_SCAN)
	{
		// Check if we have enough space for tuple header
		if ((current_offset - (uint8_t *)rom) + PCI_TUPLE_HEADER_SIZE > rom_size)
		{
			break;
		}

		uint16_t current_id = *(uint16_t *)current_offset;
		uint16_t next_offset = *(uint16_t *)(current_offset + sizeof(uint16_t));
		uint16_t data_length = *(uint16_t *)(current_offset + 2 * sizeof(uint16_t));

		// Check for end marker
		if (next_offset == PCI_TUPLE_END)
		{
			return true; // Valid end found
		}

		// Validate next offset
		if (next_offset >= rom_size)
		{
			return false; // Invalid offset
		}

		// Validate data doesn't exceed ROM bounds
		if ((current_offset - (uint8_t *)rom) + PCI_TUPLE_HEADER_SIZE + data_length > rom_size)
		{
			return false; // Data exceeds ROM size
		}

		current_offset = ((uint8_t *)rom) + next_offset;
		tuples_found++;
	}

	return false; // No valid end found or too many tuples
}

/**
 * Count total number of tuples in ROM
 *
 * @param rom Pointer to ROM data
 * @param rom_size Size of ROM data
 * @return Number of tuples found, -1 on error
 */
int count_tuples(const uint16_t *rom, size_t rom_size)
{
	if (rom == NULL)
	{
		return -1;
	}

	uint8_t *current_offset = (uint8_t *)rom;
	int tuple_count = 0;

	while (tuple_count < MAX_TUPLES_TO_SCAN)
	{
		// Check bounds
		if ((current_offset - (uint8_t *)rom) + PCI_TUPLE_HEADER_SIZE > rom_size)
		{
			break;
		}

		uint16_t next_offset = *(uint16_t *)(current_offset + sizeof(uint16_t));
		tuple_count++;

		// Check for end marker
		if (next_offset == PCI_TUPLE_END)
		{
			break;
		}

		// Validate and move to next
		if (next_offset >= rom_size)
		{
			return -1; // Invalid structure
		}

		current_offset = ((uint8_t *)rom) + next_offset;
	}

	return tuple_count;
}

/**
 * Print ROM structure for debugging
 */
void print_rom_structure(const uint16_t *rom, size_t rom_size)
{
	if (rom == NULL)
	{
		printf("ROM is NULL\n");
		return;
	}

	printf("PCI ROM Structure Analysis:\n");
	printf("ROM Size: %zu bytes\n", rom_size);
	printf("ROM Address: %p\n", (void *)rom);
	printf("\nTuple Chain:\n");

	uint8_t *current_offset = (uint8_t *)rom;
	int tuple_index = 0;

	while (tuple_index < MAX_TUPLES_TO_SCAN)
	{
		// Check bounds
		if ((current_offset - (uint8_t *)rom) + PCI_TUPLE_HEADER_SIZE > rom_size)
		{
			printf("ERROR: Not enough space for tuple header\n");
			break;
		}

		uint16_t current_id = *(uint16_t *)current_offset;
		uint16_t next_offset = *(uint16_t *)(current_offset + sizeof(uint16_t));
		uint16_t data_length = *(uint16_t *)(current_offset + 2 * sizeof(uint16_t));

		printf("Tuple %d: ID=0x%04X, NEXT=0x%04X, LEN=%u, OFFSET=%ld\n",
			   tuple_index, current_id, next_offset, data_length,
			   current_offset - (uint8_t *)rom);

		// Check for end marker
		if (next_offset == PCI_TUPLE_END)
		{
			printf("End of tuple chain (0xFFFF)\n");
			break;
		}

		// Validate and move to next
		if (next_offset >= rom_size)
		{
			printf("ERROR: Invalid next offset %u (ROM size: %zu)\n", next_offset, rom_size);
			break;
		}

		current_offset = ((uint8_t *)rom) + next_offset;
		tuple_index++;
	}

	if (tuple_index >= MAX_TUPLES_TO_SCAN)
	{
		printf("WARNING: Maximum tuple scan limit reached\n");
	}
}

// Test helper functions

/**
 * Create a simple test ROM with known tuple structure
 */
uint16_t *create_test_rom(size_t *rom_size)
{
	// Calculate required size for test ROM
	// Tuple 1: ID=0x1001, DATA="TEST1" (5 bytes + 1 padding = 6 bytes)
	// Tuple 2: ID=0x1002, DATA=42 (2 bytes)
	// Tuple 3: ID=0x1003, DATA="HELLO" (5 bytes + 1 padding = 6 bytes)
	// End marker

	size_t tuple1_size = PCI_TUPLE_HEADER_SIZE + 6; // 6 + 6 = 12 bytes
	size_t tuple2_size = PCI_TUPLE_HEADER_SIZE + 2; // 6 + 2 = 8 bytes
	size_t tuple3_size = PCI_TUPLE_HEADER_SIZE + 6; // 6 + 6 = 12 bytes
	size_t total_size = tuple1_size + tuple2_size + tuple3_size;

	uint16_t *rom = (uint16_t *)malloc(total_size);
	if (rom == NULL)
	{
		return NULL;
	}

	uint8_t *ptr = (uint8_t *)rom;

	// Tuple 1: ID=0x1001, NEXT=12, LEN=6, DATA="TEST1\0"
	*(uint16_t *)ptr = 0x1001;			  // ID
	*(uint16_t *)(ptr + 2) = tuple1_size; // NEXT offset
	*(uint16_t *)(ptr + 4) = 6;			  // LEN
	strcpy((char *)(ptr + 6), "TEST1");	  // DATA
	ptr += tuple1_size;

	// Tuple 2: ID=0x1002, NEXT=20, LEN=2, DATA=42
	*(uint16_t *)ptr = 0x1002;							// ID
	*(uint16_t *)(ptr + 2) = tuple1_size + tuple2_size; // NEXT offset
	*(uint16_t *)(ptr + 4) = 2;							// LEN
	*(uint16_t *)(ptr + 6) = 42;						// DATA
	ptr += tuple2_size;

	// Tuple 3: ID=0x1003, NEXT=0xFFFF, LEN=6, DATA="HELLO\0"
	*(uint16_t *)ptr = 0x1003;				// ID
	*(uint16_t *)(ptr + 2) = PCI_TUPLE_END; // NEXT (end marker)
	*(uint16_t *)(ptr + 4) = 6;				// LEN
	strcpy((char *)(ptr + 6), "HELLO");		// DATA

	*rom_size = total_size;
	return rom;
}

/**
 * Run a single test with detailed output
 */
void run_test(const char *test_name, bool (*test_func)(void))
{
	printf("\n=== %s ===\n", test_name);
	if (test_func())
	{
		printf("✅ PASSED\n");
	}
	else
	{
		printf("❌ FAILED\n");
		exit(1);
	}
}

// Test functions

bool test_basic_tuple_search(void)
{
	size_t rom_size;
	uint16_t *rom = create_test_rom(&rom_size);
	assert(rom != NULL);

	size_t data_len;

	// Test finding first tuple
	uint16_t *data1 = find_tuple(rom, 0x1001, &data_len);
	assert(data1 != NULL);
	assert(data_len == 6);
	assert(strcmp((char *)data1, "TEST1") == 0);
	printf("Found tuple 0x1001: \"%s\" (length: %zu)\n", (char *)data1, data_len);

	// Test finding middle tuple
	uint16_t *data2 = find_tuple(rom, 0x1002, &data_len);
	assert(data2 != NULL);
	assert(data_len == 2);
	assert(*data2 == 42);
	printf("Found tuple 0x1002: %u (length: %zu)\n", *data2, data_len);

	// Test finding last tuple
	uint16_t *data3 = find_tuple(rom, 0x1003, &data_len);
	assert(data3 != NULL);
	assert(data_len == 6);
	assert(strcmp((char *)data3, "HELLO") == 0);
	printf("Found tuple 0x1003: \"%s\" (length: %zu)\n", (char *)data3, data_len);

	free(rom);
	return true;
}

bool test_tuple_not_found(void)
{
	size_t rom_size;
	uint16_t *rom = create_test_rom(&rom_size);
	assert(rom != NULL);

	size_t data_len;

	// Test searching for non-existent tuple
	uint16_t *data = find_tuple(rom, 0x9999, &data_len);
	assert(data == NULL);
	printf("Correctly returned NULL for non-existent tuple 0x9999\n");

	free(rom);
	return true;
}

bool test_invalid_inputs(void)
{
	size_t data_len;

	// Test NULL ROM pointer
	uint16_t *data1 = find_tuple(NULL, 0x1001, &data_len);
	assert(data1 == NULL);
	printf("Correctly handled NULL ROM pointer\n");

	// Test NULL length pointer
	size_t rom_size;
	uint16_t *rom = create_test_rom(&rom_size);
	uint16_t *data2 = find_tuple(rom, 0x1001, NULL);
	assert(data2 == NULL);
	printf("Correctly handled NULL length pointer\n");

	free(rom);
	return true;
}

bool test_rom_validation(void)
{
	size_t rom_size;
	uint16_t *rom = create_test_rom(&rom_size);
	assert(rom != NULL);

	// Test valid ROM structure
	bool valid = validate_rom_structure(rom, rom_size);
	assert(valid == true);
	printf("ROM structure validation: VALID\n");

	// Test tuple counting
	int count = count_tuples(rom, rom_size);
	assert(count == 3);
	printf("Tuple count: %d\n", count);

	free(rom);
	return true;
}

bool test_empty_rom(void)
{
	// Create ROM with just end marker
	uint16_t *rom = (uint16_t *)malloc(PCI_TUPLE_HEADER_SIZE);
	*(uint16_t *)rom = 0x0000;				// ID
	*(uint16_t *)(rom + 1) = PCI_TUPLE_END; // NEXT (immediate end)
	*(uint16_t *)(rom + 2) = 0;				// LEN

	size_t data_len;
	uint16_t *data = find_tuple(rom, 0x1001, &data_len);
	assert(data == NULL);
	printf("Correctly handled empty ROM\n");

	free(rom);
	return true;
}

bool test_rom_structure_analysis(void)
{
	size_t rom_size;
	uint16_t *rom = create_test_rom(&rom_size);
	assert(rom != NULL);

	printf("Analyzing ROM structure:\n");
	print_rom_structure(rom, rom_size);

	free(rom);
	return true;
}

int main(void)
{
	printf("🧪 Testing PCI Configuration ROM Tuple Finder\n");
	printf("=============================================\n");

	// Run comprehensive test suite
	run_test("Test 1: Basic Tuple Search", test_basic_tuple_search);
	run_test("Test 2: Tuple Not Found", test_tuple_not_found);
	run_test("Test 3: Invalid Inputs", test_invalid_inputs);
	run_test("Test 4: ROM Validation", test_rom_validation);
	run_test("Test 5: Empty ROM", test_empty_rom);
	run_test("Test 6: ROM Structure Analysis", test_rom_structure_analysis);

	printf("\n🎉 All tests passed successfully!\n");
	printf("\n📋 Test Summary:\n");
	printf("   • Basic tuple search operations: ✅\n");
	printf("   • Error handling (not found): ✅\n");
	printf("   • Input validation: ✅\n");
	printf("   • ROM structure validation: ✅\n");
	printf("   • Edge cases (empty ROM): ✅\n");
	printf("   • Structure analysis tools: ✅\n");

	printf("\n🔧 To compile and run:\n");
	printf("   gcc -std=c99 -o pci_test pci_find_tuples.c && ./pci_test\n");

	return 0;
}

/*
 * 📚 ALGORITHM EXPLANATION:
 *
 * PCI CONFIGURATION ROM TUPLE STRUCTURE:
 *
 * 1. ROM ORGANIZATION:
 *    - ROM contains a linked list of configuration tuples
 *    - Each tuple has: ID (2 bytes) + NEXT (2 bytes) + LEN (2 bytes) + DATA (variable)
 *    - NEXT field contains offset from ROM start to next tuple
 *    - End of chain marked with NEXT = 0xFFFF
 *
 * 2. SEARCH ALGORITHM:
 *    a) Start at ROM beginning (offset 0)
 *    b) Parse tuple header: extract ID, NEXT, LEN
 *    c) Compare ID with target ID
 *    d) If match: return pointer to data portion
 *    e) If no match: follow NEXT pointer to continue
 *    f) Repeat until match found or 0xFFFF encountered
 *
 * 3. MEMORY LAYOUT EXAMPLE:
 *    ROM[0x0000]: [ID=0x1001][NEXT=0x000C][LEN=0x0006][DATA="TEST1\0"]
 *    ROM[0x000C]: [ID=0x1002][NEXT=0x0014][LEN=0x0002][DATA=42    ]
 *    ROM[0x0014]: [ID=0x1003][NEXT=0xFFFF][LEN=0x0006][DATA="HELLO\0"]
 *
 * 4. SAFETY MECHANISMS:
 *    - Bounds checking: validate offsets within ROM size
 *    - Loop protection: maximum iteration limit
 *    - Null pointer validation: check input parameters
 *    - End marker detection: proper chain termination
 *
 * 5. FIXED ISSUES FROM ORIGINAL CODE:
 *    - Fixed wrong loop condition (was > instead of <)
 *    - Added missing semicolon
 *    - Fixed incorrect pointer dereferencing
 *    - Added proper bounds checking
 *    - Improved variable naming and code structure
 *    - Added comprehensive error handling
 *
 * 6. PERFORMANCE CHARACTERISTICS:
 *    - Time Complexity: O(n) where n = number of tuples
 *    - Space Complexity: O(1) - no additional memory allocation
 *    - Early termination on match found
 *    - Linear scan through tuple chain
 *
 * 7. REAL-WORLD USAGE:
 *    - PCI device configuration ROM parsing
 *    - Option ROM data extraction
 *    - Device capability discovery
 *    - Hardware initialization parameters
 *
 * ⏱️ COMPLEXITY:
 * - Time: O(n) where n is the number of tuples in ROM
 * - Space: O(1) constant space usage
 * - Safety: Multiple bounds checks and validation layers
 */

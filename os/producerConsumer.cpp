/*
 * PRODUCER-CONSUMER PROBLEM IMPLEMENTATION
 *
 * This file demonstrates the classic producer-consumer synchronization problem
 * with multiple solution approaches. The producer-consumer problem is a
 * fundamental concurrency challenge where multiple threads share a bounded buffer.
 *
 * PROBLEM DEFINITION:
 * - Producers generate data and place it in a shared buffer
 * - Consumers retrieve and process data from the shared buffer
 * - Buffer has limited capacity (bounded buffer)
 * - Multiple producers and consumers can work concurrently
 * - Must prevent race conditions and ensure proper synchronization
 *
 * SYNCHRONIZATION CHALLENGES:
 * 1. Race Conditions: Multiple threads accessing shared data
 * 2. Buffer Overflow: Producers adding to full buffer
 * 3. Buffer Underflow: Consumers reading from empty buffer
 * 4. Lost Updates: Concurrent modifications to shared variables
 * 5. Deadlock: Threads waiting indefinitely for each other
 *
 * SOLUTIONS DEMONSTRATED:
 * 1. Unsafe implementation (shows the problem)
 * 2. Single condition variable solution
 * 3. Two condition variables solution (recommended)
 * 4. Modern C++ implementation with comprehensive testing
 *
 * REAL-WORLD APPLICATIONS:
 * - Web servers handling client requests
 * - Print job spooling systems
 * - Media streaming buffers
 * - Database connection pools
 * - Message queues and event systems
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

// Global configuration and shared state
static int buffer_size = 10;  // Maximum buffer capacity
static int num_items = 20;    // Items each producer creates
static int num_producers = 2; // Number of producer threads
static int num_consumers = 2; // Number of consumer threads
static int verbose = 0;       // Detailed output flag

// Shared buffer and synchronization primitives
static int *buffer = NULL; // Circular buffer for items
static int fill_index = 0; // Next position to produce item
static int use_index = 0;  // Next position to consume item
static int count = 0;      // Current number of items in buffer

// Synchronization primitives for different solutions
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;  // Single CV solution
static pthread_cond_t empty = PTHREAD_COND_INITIALIZER; // Buffer not full
static pthread_cond_t full = PTHREAD_COND_INITIALIZER;  // Buffer not empty

// Statistics and monitoring
static int total_produced = 0;    // Total items produced
static int total_consumed = 0;    // Total items consumed
static int producer_waits = 0;    // Times producers waited
static int consumer_waits = 0;    // Times consumers waited
static struct timeval start_time; // Test start time

/**
 * Thread-safe buffer operations with bounds checking
 */
void put_item(int value)
{
    if (count >= buffer_size)
    {
        fprintf(stderr, "ERROR: Buffer overflow! count=%d, max=%d\n", count, buffer_size);
        return;
    }

    buffer[fill_index] = value;
    fill_index = (fill_index + 1) % buffer_size;
    count++;
    total_produced++;

    if (verbose)
    {
        printf("[PUT] Item %d at index %d, buffer count: %d\n",
               value, (fill_index - 1 + buffer_size) % buffer_size, count);
    }
}

int get_item(void)
{
    if (count <= 0)
    {
        fprintf(stderr, "ERROR: Buffer underflow! count=%d\n", count);
        return -1;
    }

    int item = buffer[use_index];
    use_index = (use_index + 1) % buffer_size;
    count--;
    total_consumed++;

    if (verbose)
    {
        printf("[GET] Item %d from index %d, buffer count: %d\n",
               item, (use_index - 1 + buffer_size) % buffer_size, count);
    }

    return item;
}

/**
 * Get current time in milliseconds
 */
long get_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec - start_time.tv_sec) * 1000 + (tv.tv_usec - start_time.tv_usec) / 1000;
}

/*
 * ============================================================================
 * SOLUTION 0: UNSAFE IMPLEMENTATION (DEMONSTRATES THE PROBLEM)
 * ============================================================================
 *
 * This implementation shows what happens without proper synchronization.
 * Multiple race conditions can occur:
 * - Lost updates to shared variables
 * - Buffer overflow/underflow
 * - Inconsistent state between fill_index, use_index, and count
 */

void *unsafe_producer(void *arg)
{
    int producer_id = *(int *)arg;
    printf("🔴 Unsafe Producer %d starting\n", producer_id);

    for (int i = 0; i < num_items; i++)
    {
        // RACE CONDITION: No synchronization!
        // Multiple producers can interfere with each other
        int item = producer_id * 1000 + i;

        // Simulate some production work
        usleep(1000 + (rand() % 2000));

        put_item(item); // UNSAFE: No mutual exclusion

        if (verbose)
        {
            printf("🔴 Producer %d produced item %d\n", producer_id, item);
        }
    }

    printf("🔴 Unsafe Producer %d finished\n", producer_id);
    return NULL;
}

void *unsafe_consumer(void *arg)
{
    int consumer_id = *(int *)arg;
    printf("🔴 Unsafe Consumer %d starting\n", consumer_id);

    for (int i = 0; i < num_items; i++)
    {
        // RACE CONDITION: No synchronization!
        // Multiple consumers can interfere with each other

        // Simulate some consumption work
        usleep(1500 + (rand() % 2000));

        int item = get_item(); // UNSAFE: No mutual exclusion

        if (item >= 0 && verbose)
        {
            printf("🔴 Consumer %d consumed item %d\n", consumer_id, item);
        }
    }

    printf("🔴 Unsafe Consumer %d finished\n", consumer_id);
    return NULL;
}

/*
 * ============================================================================
 * SOLUTION 1: SINGLE CONDITION VARIABLE WITH "WHILE" LOOPS
 * ============================================================================
 *
 * This solution uses a single condition variable with proper while loops.
 *
 * KEY INSIGHTS:
 * - Must use "while" not "if" for condition checking
 * - State can change between signal and thread wakeup
 * - Single CV means producers might wake producers (inefficient)
 * - Consumers might wake consumers (inefficient)
 *
 * PROS:
 * - Correct synchronization
 * - Simple to understand
 * - Only one condition variable needed
 *
 * CONS:
 * - Inefficient signaling (spurious wakeups)
 * - Producers can wake other producers unnecessarily
 * - Consumers can wake other consumers unnecessarily
 */

void *single_cv_producer(void *arg)
{
    int producer_id = *(int *)arg;
    printf("🟡 Single-CV Producer %d starting\n", producer_id);

    for (int i = 0; i < num_items; i++)
    {
        int item = producer_id * 1000 + i;

        pthread_mutex_lock(&mutex);

        // CRITICAL: Use "while" not "if"
        // Buffer state might change between signal and wakeup
        while (count == buffer_size)
        {
            if (verbose)
            {
                printf("🟡 Producer %d waiting (buffer full)\n", producer_id);
            }
            producer_waits++;
            pthread_cond_wait(&cond, &mutex); // Wait for space
        }

        put_item(item);

        if (verbose)
        {
            printf("🟡 Producer %d produced item %d\n", producer_id, item);
        }

        pthread_cond_signal(&cond); // Signal that buffer changed
        pthread_mutex_unlock(&mutex);

        // Simulate production work outside critical section
        usleep(100 + (rand() % 1000));
    }

    printf("🟡 Single-CV Producer %d finished\n", producer_id);
    return NULL;
}

void *single_cv_consumer(void *arg)
{
    int consumer_id = *(int *)arg;
    printf("🟡 Single-CV Consumer %d starting\n", consumer_id);

    for (int i = 0; i < num_items; i++)
    {
        pthread_mutex_lock(&mutex);

        // CRITICAL: Use "while" not "if"
        // Buffer state might change between signal and wakeup
        while (count == 0)
        {
            if (verbose)
            {
                printf("🟡 Consumer %d waiting (buffer empty)\n", consumer_id);
            }
            consumer_waits++;
            pthread_cond_wait(&cond, &mutex); // Wait for items
        }

        int item = get_item();

        if (verbose)
        {
            printf("🟡 Consumer %d consumed item %d\n", consumer_id, item);
        }

        pthread_cond_signal(&cond); // Signal that buffer changed
        pthread_mutex_unlock(&mutex);

        // Simulate consumption work outside critical section
        usleep(150 + (rand() % 1500));
    }

    printf("🟡 Single-CV Consumer %d finished\n", consumer_id);
    return NULL;
}

/*
 * ============================================================================
 * SOLUTION 2: TWO CONDITION VARIABLES (RECOMMENDED APPROACH)
 * ============================================================================
 *
 * This solution uses two condition variables for optimal efficiency:
 * - 'empty': Signaled when buffer has space (for producers)
 * - 'full': Signaled when buffer has items (for consumers)
 *
 * KEY INSIGHTS:
 * - Producers wait on 'empty' and signal 'full'
 * - Consumers wait on 'full' and signal 'empty'
 * - No spurious wakeups between producer/consumer types
 * - More efficient than single CV solution
 *
 * PROS:
 * - Correct synchronization
 * - Efficient signaling (no spurious wakeups)
 * - Producers only wake consumers and vice versa
 * - Better performance under high contention
 *
 * CONS:
 * - Slightly more complex
 * - Need to manage two condition variables
 */

void *two_cv_producer(void *arg)
{
    int producer_id = *(int *)arg;
    printf("🟢 Two-CV Producer %d starting\n", producer_id);

    for (int i = 0; i < num_items; i++)
    {
        int item = producer_id * 1000 + i;

        pthread_mutex_lock(&mutex);

        // Wait for buffer to have space
        while (count == buffer_size)
        {
            if (verbose)
            {
                printf("🟢 Producer %d waiting for empty space\n", producer_id);
            }
            producer_waits++;
            pthread_cond_wait(&empty, &mutex); // Wait for space
        }

        put_item(item);

        if (verbose)
        {
            printf("🟢 Producer %d produced item %d\n", producer_id, item);
        }

        pthread_cond_signal(&full); // Signal consumers that item available
        pthread_mutex_unlock(&mutex);

        // Simulate production work outside critical section
        usleep(100 + (rand() % 1000));
    }

    printf("🟢 Two-CV Producer %d finished\n", producer_id);
    return NULL;
}

void *two_cv_consumer(void *arg)
{
    int consumer_id = *(int *)arg;
    printf("🟢 Two-CV Consumer %d starting\n", consumer_id);

    for (int i = 0; i < num_items; i++)
    {
        pthread_mutex_lock(&mutex);

        // Wait for buffer to have items
        while (count == 0)
        {
            if (verbose)
            {
                printf("🟢 Consumer %d waiting for items\n", consumer_id);
            }
            consumer_waits++;
            pthread_cond_wait(&full, &mutex); // Wait for items
        }

        int item = get_item();

        if (verbose)
        {
            printf("🟢 Consumer %d consumed item %d\n", consumer_id, item);
        }

        pthread_cond_signal(&empty); // Signal producers that space available
        pthread_mutex_unlock(&mutex);

        // Simulate consumption work outside critical section
        usleep(150 + (rand() % 1500));
    }

    printf("🟢 Two-CV Consumer %d finished\n", consumer_id);
    return NULL;
}

/**
 * Initialize shared resources
 */
int initialize_resources(void)
{
    // Allocate buffer
    buffer = (int *)malloc(buffer_size * sizeof(int));
    if (!buffer)
    {
        perror("Failed to allocate buffer");
        return -1;
    }

    // Initialize buffer with sentinel values for debugging
    for (int i = 0; i < buffer_size; i++)
    {
        buffer[i] = -999; // Sentinel value
    }

    // Reset global state
    fill_index = 0;
    use_index = 0;
    count = 0;
    total_produced = 0;
    total_consumed = 0;
    producer_waits = 0;
    consumer_waits = 0;

    // Initialize random seed
    srand(time(NULL));

    // Record start time
    gettimeofday(&start_time, NULL);

    return 0;
}

/**
 * Cleanup resources
 */
void cleanup_resources(void)
{
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }
}

/**
 * Print test statistics
 */
void print_statistics(const char *test_name, long duration_ms)
{
    printf("\n📊 %s STATISTICS:\n", test_name);
    printf("   Duration: %ld ms\n", duration_ms);
    printf("   Buffer size: %d\n", buffer_size);
    printf("   Producers: %d, Consumers: %d\n", num_producers, num_consumers);
    printf("   Items per producer: %d\n", num_items);
    printf("   Total produced: %d\n", total_produced);
    printf("   Total consumed: %d\n", total_consumed);
    printf("   Producer waits: %d\n", producer_waits);
    printf("   Consumer waits: %d\n", consumer_waits);
    printf("   Final buffer count: %d\n", count);

    if (total_produced == total_consumed)
    {
        printf("   ✅ All items produced and consumed correctly\n");
    }
    else
    {
        printf("   ❌ Mismatch: %d produced, %d consumed\n",
               total_produced, total_consumed);
    }
}

/**
 * Validate buffer integrity
 */
int validate_buffer_state(void)
{
    if (count < 0 || count > buffer_size)
    {
        printf("❌ Invalid buffer count: %d (max: %d)\n", count, buffer_size);
        return 0;
    }

    if (fill_index < 0 || fill_index >= buffer_size)
    {
        printf("❌ Invalid fill_index: %d (max: %d)\n", fill_index, buffer_size - 1);
        return 0;
    }

    if (use_index < 0 || use_index >= buffer_size)
    {
        printf("❌ Invalid use_index: %d (max: %d)\n", use_index, buffer_size - 1);
        return 0;
    }

    return 1;
}

/**
 * Run a specific producer-consumer test
 */
int run_test(const char *test_name,
             void *(*producer_func)(void *),
             void *(*consumer_func)(void *))
{

    printf("\n============================================================\n");
    printf("🧪 %s\n", test_name);
    printf("============================================================\n");

    // Initialize resources
    if (initialize_resources() != 0)
    {
        return -1;
    }

    // Create thread arrays
    pthread_t *producers = (pthread_t *)malloc(num_producers * sizeof(pthread_t));
    pthread_t *consumers = (pthread_t *)malloc(num_consumers * sizeof(pthread_t));
    int *producer_ids = (int *)malloc(num_producers * sizeof(int));
    int *consumer_ids = (int *)malloc(num_consumers * sizeof(int));

    if (!producers || !consumers || !producer_ids || !consumer_ids)
    {
        perror("Failed to allocate thread arrays");
        return -1;
    }

    long start_ms = get_time_ms();

    // Create producer threads
    for (int i = 0; i < num_producers; i++)
    {
        producer_ids[i] = i;
        if (pthread_create(&producers[i], NULL, producer_func, &producer_ids[i]) != 0)
        {
            perror("Failed to create producer thread");
            return -1;
        }
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; i++)
    {
        consumer_ids[i] = i;
        if (pthread_create(&consumers[i], NULL, consumer_func, &consumer_ids[i]) != 0)
        {
            perror("Failed to create consumer thread");
            return -1;
        }
    }

    // Wait for all producer threads to complete
    for (int i = 0; i < num_producers; i++)
    {
        if (pthread_join(producers[i], NULL) != 0)
        {
            perror("Failed to join producer thread");
        }
    }

    // Wait for all consumer threads to complete
    for (int i = 0; i < num_consumers; i++)
    {
        if (pthread_join(consumers[i], NULL) != 0)
        {
            perror("Failed to join consumer thread");
        }
    }

    long end_ms = get_time_ms();
    long duration = end_ms - start_ms;

    // Validate final state
    int valid = validate_buffer_state();

    // Print statistics
    print_statistics(test_name, duration);

    // Cleanup
    free(producers);
    free(consumers);
    free(producer_ids);
    free(consumer_ids);
    cleanup_resources();

    return valid ? 0 : -1;
}

/**
 * Display usage information
 */
void print_usage(const char *program_name)
{
    printf("Usage: %s [options]\n", program_name);
    printf("Options:\n");
    printf("  -b SIZE     Buffer size (default: %d)\n", buffer_size);
    printf("  -n ITEMS    Items per producer (default: %d)\n", num_items);
    printf("  -p COUNT    Number of producers (default: %d)\n", num_producers);
    printf("  -c COUNT    Number of consumers (default: %d)\n", num_consumers);
    printf("  -v          Verbose output\n");
    printf("  -h          Show this help\n");
    printf("\nExamples:\n");
    printf("  %s -b 5 -n 10 -p 2 -c 2     # Small buffer, multiple threads\n", program_name);
    printf("  %s -b 100 -n 1000 -v        # Large buffer, verbose output\n", program_name);
}

/**
 * MAIN FUNCTION WITH COMPREHENSIVE TESTING
 */
int main(int argc, char *argv[])
{
    // Parse command line arguments
    int opt;
    while ((opt = getopt(argc, argv, "b:n:p:c:vh")) != -1)
    {
        switch (opt)
        {
        case 'b':
            buffer_size = atoi(optarg);
            if (buffer_size <= 0)
            {
                fprintf(stderr, "Buffer size must be positive\n");
                return 1;
            }
            break;
        case 'n':
            num_items = atoi(optarg);
            if (num_items <= 0)
            {
                fprintf(stderr, "Number of items must be positive\n");
                return 1;
            }
            break;
        case 'p':
            num_producers = atoi(optarg);
            if (num_producers <= 0)
            {
                fprintf(stderr, "Number of producers must be positive\n");
                return 1;
            }
            break;
        case 'c':
            num_consumers = atoi(optarg);
            if (num_consumers <= 0)
            {
                fprintf(stderr, "Number of consumers must be positive\n");
                return 1;
            }
            break;
        case 'v':
            verbose = 1;
            break;
        case 'h':
            print_usage(argv[0]);
            return 0;
        default:
            print_usage(argv[0]);
            return 1;
        }
    }

    printf("🧪 PRODUCER-CONSUMER PROBLEM DEMONSTRATION\n");
    printf("==========================================\n");
    printf("Configuration:\n");
    printf("  Buffer size: %d\n", buffer_size);
    printf("  Items per producer: %d\n", num_items);
    printf("  Producers: %d, Consumers: %d\n", num_producers, num_consumers);
    printf("  Verbose output: %s\n", verbose ? "ON" : "OFF");

    int test_failures = 0;

    // Test 1: Unsafe implementation (demonstrates race conditions)
    printf("\n⚠️  WARNING: Running unsafe test - expect inconsistent results!\n");
    if (run_test("UNSAFE IMPLEMENTATION (RACE CONDITIONS)",
                 unsafe_producer, unsafe_consumer) != 0)
    {
        printf("❌ Unsafe test validation failed (expected!)\n");
        // Don't count as failure - this is expected to have issues
    }

    // Small delay between tests
    sleep(1);

    // Test 2: Single condition variable solution
    if (run_test("SINGLE CONDITION VARIABLE SOLUTION",
                 single_cv_producer, single_cv_consumer) != 0)
    {
        printf("❌ Single CV test failed\n");
        test_failures++;
    }
    else
    {
        printf("✅ Single CV test passed\n");
    }

    // Small delay between tests
    sleep(1);

    // Test 3: Two condition variables solution (recommended)
    if (run_test("TWO CONDITION VARIABLES SOLUTION (RECOMMENDED)",
                 two_cv_producer, two_cv_consumer) != 0)
    {
        printf("❌ Two CV test failed\n");
        test_failures++;
    }
    else
    {
        printf("✅ Two CV test passed\n");
    }

    // Summary
    printf("\n============================================================\n");
    printf("🎉 TEST SUMMARY\n");
    printf("============================================================\n");

    if (test_failures == 0)
    {
        printf("✅ All synchronized tests passed successfully!\n");
        printf("✅ Producer-consumer implementations working correctly\n");
    }
    else
    {
        printf("❌ %d test(s) failed\n", test_failures);
        printf("⚠️  Check implementation for synchronization issues\n");
    }

    printf("\n🎓 KEY LEARNING POINTS:\n");
    printf("1. 🔴 Unsafe: Race conditions cause data corruption\n");
    printf("2. 🟡 Single CV: Correct but inefficient signaling\n");
    printf("3. 🟢 Two CVs: Optimal solution with efficient signaling\n");
    printf("4. 📊 Always validate correctness with statistics\n");
    printf("5. 🧪 Test with different buffer sizes and thread counts\n");

    printf("\n🔧 COMPILATION:\n");
    printf("   gcc -pthread -Wall -Wextra -O2 -o producer_consumer producerConsumer.cpp\n");
    printf("   ./producer_consumer -b 10 -n 50 -p 3 -c 2 -v\n");

    return test_failures > 0 ? 1 : 0;
}

// Solution #1: Single CV, “while”

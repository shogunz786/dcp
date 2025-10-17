#!/bin/bash

# PRODUCER-CONSUMER TEST SCRIPT
# Comprehensive testing of different scenarios

echo "🧪 PRODUCER-CONSUMER COMPREHENSIVE TEST SUITE"
echo "=============================================="

# Test 1: Basic functionality
echo ""
echo "📋 Test 1: Basic functionality (balanced producers/consumers)"
./producer_consumer -b 10 -n 10 -p 2 -c 2
if [ $? -eq 0 ]; then
    echo "✅ Test 1 PASSED"
else
    echo "❌ Test 1 FAILED"
fi

# Test 2: Small buffer, high contention
echo ""
echo "📋 Test 2: Small buffer with high contention"
./producer_consumer -b 2 -n 8 -p 3 -c 2
if [ $? -eq 0 ]; then
    echo "✅ Test 2 PASSED"
else
    echo "❌ Test 2 FAILED"
fi

# Test 3: Many producers, few consumers
echo ""
echo "📋 Test 3: Many producers, few consumers"
./producer_consumer -b 5 -n 5 -p 4 -c 1
if [ $? -eq 0 ]; then
    echo "✅ Test 3 PASSED"
else
    echo "❌ Test 3 FAILED"
fi

# Test 4: Few producers, many consumers
echo ""
echo "📋 Test 4: Few producers, many consumers"
./producer_consumer -b 5 -n 10 -p 1 -c 4
if [ $? -eq 0 ]; then
    echo "✅ Test 4 PASSED"
else
    echo "❌ Test 4 FAILED"
fi

# Test 5: Large buffer, minimal contention
echo ""
echo "📋 Test 5: Large buffer with minimal contention"
./producer_consumer -b 50 -n 20 -p 2 -c 2
if [ $? -eq 0 ]; then
    echo "✅ Test 5 PASSED"
else
    echo "❌ Test 5 FAILED"
fi

# Test 6: Single producer, single consumer
echo ""
echo "📋 Test 6: Single producer, single consumer"
./producer_consumer -b 3 -n 15 -p 1 -c 1
if [ $? -eq 0 ]; then
    echo "✅ Test 6 PASSED"
else
    echo "❌ Test 6 FAILED"
fi

echo ""
echo "🎉 ALL TESTS COMPLETED!"
echo ""
echo "🔧 USAGE EXAMPLES:"
echo "   Basic test:           ./producer_consumer"
echo "   Verbose output:       ./producer_consumer -v"
echo "   Custom parameters:    ./producer_consumer -b 5 -n 20 -p 3 -c 2 -v"
echo "   Small buffer test:    ./producer_consumer -b 2 -n 10 -p 4 -c 2"
echo "   Help:                 ./producer_consumer -h"
echo ""
echo "📚 EDUCATIONAL POINTS:"
echo "   - Unsafe version shows race conditions and data corruption"
echo "   - Single CV version is correct but can have spurious wakeups"
echo "   - Two CV version is optimal with efficient signaling"
echo "   - Buffer size affects contention and waiting patterns"
echo "   - Producer/consumer ratios affect performance characteristics"
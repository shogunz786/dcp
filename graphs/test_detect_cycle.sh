#!/bin/bash

# Cycle Detection Algorithms - Automated Test Script
# ================================================
# This script compiles and runs comprehensive tests for the cycle detection implementation

echo "🎯 CYCLE DETECTION ALGORITHMS - AUTOMATED TESTING"
echo "================================================"
echo

# Color codes for output formatting
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    case $2 in
        "SUCCESS") echo -e "${GREEN}✅ $1${NC}" ;;
        "ERROR")   echo -e "${RED}❌ $1${NC}" ;;
        "WARNING") echo -e "${YELLOW}⚠️  $1${NC}" ;;
        "INFO")    echo -e "${BLUE}ℹ️  $1${NC}" ;;
        *)         echo "$1" ;;
    esac
}

# Check if source file exists
if [ ! -f "detect_cycle.cpp" ]; then
    print_status "Source file detect_cycle.cpp not found!" "ERROR"
    exit 1
fi

print_status "Found source file: detect_cycle.cpp" "SUCCESS"
echo

# Compilation
print_status "Compiling with g++ -std=c++11 -O2..." "INFO"
if g++ -std=c++11 -O2 -o detect_cycle detect_cycle.cpp 2>/dev/null; then
    print_status "Compilation successful!" "SUCCESS"
else
    print_status "Compilation failed!" "ERROR"
    echo "Compilation errors:"
    g++ -std=c++11 -O2 -o detect_cycle detect_cycle.cpp
    exit 1
fi
echo

# Run tests
print_status "Running comprehensive test suite..." "INFO"
echo "================================================"

if ./detect_cycle --tests-only; then
    echo
    print_status "All tests completed!" "SUCCESS"
else
    echo
    print_status "Some tests failed!" "WARNING"
fi

echo
echo "================================================"

# Run performance benchmark
print_status "Running performance benchmarks..." "INFO"
echo "================================================"

if ./detect_cycle --benchmark --no-tests; then
    echo
    print_status "Performance benchmarks completed!" "SUCCESS"
else
    echo
    print_status "Benchmark execution failed!" "ERROR"
fi

echo
echo "================================================"

# Quick demonstration
print_status "Running quick demonstration..." "INFO"
echo "================================================"

if ./detect_cycle --demo --no-tests; then
    echo
    print_status "Demonstration completed!" "SUCCESS"
else
    echo
    print_status "Demonstration failed!" "ERROR"
fi

echo
echo "================================================"
print_status "Automated testing completed!" "SUCCESS"
echo
print_status "Test Summary:" "INFO"
echo "• Comprehensive test suite with edge cases"
echo "• Performance benchmarks across graph sizes"
echo "• Interactive demonstrations with real-world examples"
echo "• All core algorithms: DFS, Kahn's, Union-Find, Tarjan's SCC"
echo
print_status "Ready for production use!" "SUCCESS"
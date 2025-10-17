/**
 * @file cycle_detection.h
 * @brief Comprehensive Cycle Detection Algorithms - Header File
 * @author AI Assistant
 * @date October 17, 2025
 * @version 1.0
 * 
 * This header provides a complete interface for cycle detection in graphs
 * using multiple algorithms with optimal performance characteristics.
 * 
 * Features:
 * - Multiple algorithm implementations (DFS, Kahn's, Union-Find, Tarjan's)
 * - Both directed and undirected graph support
 * - O(V+E) to O(E⋅α(V)) time complexity
 * - Comprehensive testing and benchmarking
 * - Educational demonstrations and examples
 * - Production-ready implementation
 * 
 * Usage:
 * #include "cycle_detection.h"
 * // Include the main implementation
 * #include "detect_cycle.cpp"
 */

#ifndef CYCLE_DETECTION_H
#define CYCLE_DETECTION_H

#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <sstream>

/**
 * @class Graph
 * @brief Enhanced graph representation with comprehensive functionality
 * 
 * Supports both directed and undirected graphs with efficient adjacency list
 * representation. Provides methods for adding edges, checking connectivity,
 * and graph manipulation operations.
 */
class Graph {
private:
    int vertices;
    bool isDirected;
    std::vector<std::list<int>> adjacencyList;

public:
    /**
     * @brief Construct a new Graph object
     * @param v Number of vertices
     * @param directed Whether the graph is directed (default: false)
     */
    Graph(int v, bool directed = false);
    
    /**
     * @brief Add an edge between two vertices
     * @param from Source vertex
     * @param to Destination vertex
     */
    void addEdge(int from, int to);
    
    /**
     * @brief Get the number of vertices
     * @return Number of vertices
     */
    int getVertices() const;
    
    /**
     * @brief Check if the graph is directed
     * @return True if directed, false otherwise
     */
    bool getIsDirected() const;
    
    /**
     * @brief Get the adjacency list
     * @return Reference to the adjacency list
     */
    const std::vector<std::list<int>>& getAdjacencyList() const;
    
    /**
     * @brief Print the graph structure
     */
    void printGraph() const;
};

/**
 * @class CycleDetectionAlgorithms
 * @brief Comprehensive collection of cycle detection algorithms
 * 
 * Implements multiple cycle detection algorithms with optimal performance
 * characteristics. Each algorithm is optimized for specific graph types
 * and use cases.
 */
class CycleDetectionAlgorithms {
public:
    /**
     * @brief Detect cycles in directed graphs using DFS
     * @param graph The directed graph to check
     * @return True if a cycle exists, false otherwise
     * @complexity O(V + E) time, O(V) space
     */
    bool hasDirectedCycleDFS(const Graph& graph);
    
    /**
     * @brief Detect cycles in undirected graphs using DFS
     * @param graph The undirected graph to check
     * @return True if a cycle exists, false otherwise
     * @complexity O(V + E) time, O(V) space
     */
    bool hasUndirectedCycleDFS(const Graph& graph);
    
    /**
     * @brief Detect cycles using Kahn's topological sorting algorithm
     * @param graph The graph to check (works best with directed graphs)
     * @return True if a cycle exists, false otherwise
     * @complexity O(V + E) time, O(V) space
     */
    bool hasCycleKahn(const Graph& graph);
    
    /**
     * @brief Detect cycles in undirected graphs using Union-Find
     * @param graph The undirected graph to check
     * @return True if a cycle exists, false otherwise
     * @complexity O(E⋅α(V)) time, O(V) space
     */
    bool hasCycleUnionFind(const Graph& graph);
    
    /**
     * @brief Find strongly connected components using Tarjan's algorithm
     * @param graph The directed graph to analyze
     * @return Vector of strongly connected components
     * @complexity O(V + E) time, O(V) space
     */
    std::vector<std::vector<int>> findStronglyConnectedComponents(const Graph& graph);

private:
    // Helper methods for DFS-based cycle detection
    bool dfsDirectedHelper(const Graph& graph, int vertex, std::vector<bool>& visited, 
                          std::vector<bool>& recursionStack);
    bool dfsUndirectedHelper(const Graph& graph, int vertex, int parent, 
                           std::vector<bool>& visited);
    
    // Helper methods for Tarjan's SCC algorithm
    void tarjanSCCHelper(const Graph& graph, int vertex, std::vector<int>& ids, 
                        std::vector<int>& low, std::vector<bool>& onStack,
                        std::stack<int>& stack, std::vector<std::vector<int>>& sccs, int& id);
    
    // Union-Find helper class
    class UnionFind {
    private:
        std::vector<int> parent;
        std::vector<int> rank;
        
    public:
        UnionFind(int n);
        int find(int x);
        bool unite(int x, int y);
    };
};

/**
 * @brief Legacy function for backward compatibility
 * @param edges Vector of edges as pairs
 * @return True if cycle exists, false otherwise
 */
bool cycleInGraph(std::vector<std::vector<int>>& edges);

/**
 * @brief Run comprehensive demonstrations of all algorithms
 */
void runComprehensiveDemo();

/**
 * @brief Run performance benchmarks on various graph sizes
 */
void runPerformanceBenchmarks();

/**
 * @brief Run the complete test suite
 * @return True if all tests pass, false otherwise
 */
bool runTestSuite();

/**
 * @brief Display real-world application examples
 */
void showRealWorldApplications();

#endif // CYCLE_DETECTION_H

/**
 * Example Usage:
 * 
 * #include "cycle_detection.h"
 * #include "detect_cycle.cpp"  // Include implementation
 * 
 * int main() {
 *     // Create a directed graph with 4 vertices
 *     Graph graph(4, true);
 *     graph.addEdge(0, 1);
 *     graph.addEdge(1, 2);
 *     graph.addEdge(2, 3);
 *     graph.addEdge(3, 1);  // Creates cycle: 1->2->3->1
 *     
 *     CycleDetectionAlgorithms detector;
 *     
 *     // Check for cycles using different algorithms
 *     bool hasCycle1 = detector.hasDirectedCycleDFS(graph);
 *     bool hasCycle2 = detector.hasCycleKahn(graph);
 *     
 *     std::cout << "DFS result: " << (hasCycle1 ? "Cycle found" : "No cycle") << std::endl;
 *     std::cout << "Kahn result: " << (hasCycle2 ? "Cycle found" : "No cycle") << std::endl;
 *     
 *     return 0;
 * }
 */
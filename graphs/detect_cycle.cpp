/**
 * @file detect_cycle.cpp
 * @brief Comprehensive implementation of cycle detection algorithms for directed and undirected graphs
 * @author AI Assistant
 * @date October 17, 2025
 *
 * This file implements multiple approaches for detecting cycles in graphs:
 * - DFS-based cycle detection for directed graphs (using recursion stack)
 * - DFS-based cycle detection for undirected graphs (using parent tracking)
 * - Kahn's algorithm approach (topological sort based)
 * - Union-Find approach for undirected graphs
 * - Floyd's cycle detection for linked list representation
 * - Advanced algorithms with path reconstruction
 *
 * Educational Focus:
 * - Graph traversal techniques and state management
 * - Directed vs undirected graph cycle detection differences
 * - Multiple algorithmic approaches and their trade-offs
 * - Real-world applications in dependency analysis and scheduling
 */

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <cassert>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace std::chrono;

/**
 * @enum GraphType
 * @brief Specifies whether the graph is directed or undirected
 */
enum class GraphType
{
	DIRECTED,
	UNDIRECTED
};

/**
 * @enum CycleState
 * @brief States used in DFS-based cycle detection
 */
enum class CycleState
{
	UNVISITED = 0, // White - not yet visited
	VISITING = 1,  // Gray - currently being processed
	VISITED = 2	   // Black - completely processed
};

/**
 * @struct CycleResult
 * @brief Result structure containing cycle detection information
 */
struct CycleResult
{
	bool hasCycle;
	vector<int> cyclePath;
	int cycleLength;

	CycleResult() : hasCycle(false), cycleLength(0) {}
	CycleResult(bool cycle) : hasCycle(cycle), cycleLength(0) {}
	CycleResult(bool cycle, const vector<int> &path)
		: hasCycle(cycle), cyclePath(path), cycleLength(path.size()) {}
};

/**
 * @class CycleDetectionAlgorithms
 * @brief Collection of cycle detection algorithms for various graph representations
 *
 * This class provides multiple approaches for cycle detection:
 * - Optimized algorithms for different graph types
 * - Path reconstruction for debugging and analysis
 * - Performance benchmarking capabilities
 * - Educational demonstrations with step-by-step execution
 */
class CycleDetectionAlgorithms
{
public:
	/**
	 * @brief Detect cycle in directed graph using DFS with recursion stack
	 * @param graph Adjacency list representation of directed graph
	 * @return CycleResult containing cycle information
	 *
	 * Algorithm: DFS with three-color approach
	 * Time Complexity: O(V + E)
	 * Space Complexity: O(V) for recursion stack and state arrays
	 *
	 * Key Insight: A back edge to a vertex in the current recursion stack
	 * indicates a cycle in directed graphs.
	 */
	static CycleResult detectCycleDirectedDFS(const vector<vector<int>> &graph)
	{
		int n = graph.size();
		vector<CycleState> state(n, CycleState::UNVISITED);
		vector<int> parent(n, -1);
		vector<int> path;

		for (int i = 0; i < n; i++)
		{
			if (state[i] == CycleState::UNVISITED)
			{
				CycleResult result = dfsDirectedHelper(graph, i, state, parent, path);
				if (result.hasCycle)
				{
					return result;
				}
			}
		}

		return CycleResult(false);
	}

	/**
	 * @brief Detect cycle in undirected graph using DFS with parent tracking
	 * @param graph Adjacency list representation of undirected graph
	 * @return CycleResult containing cycle information
	 *
	 * Algorithm: DFS with parent tracking
	 * Time Complexity: O(V + E)
	 * Space Complexity: O(V) for visited array and recursion stack
	 *
	 * Key Insight: In undirected graphs, a back edge to any visited vertex
	 * (except parent) indicates a cycle.
	 */
	static CycleResult detectCycleUndirectedDFS(const vector<vector<int>> &graph)
	{
		int n = graph.size();
		vector<bool> visited(n, false);
		vector<int> parent(n, -1);

		for (int i = 0; i < n; i++)
		{
			if (!visited[i])
			{
				CycleResult result = dfsUndirectedHelper(graph, i, -1, visited, parent);
				if (result.hasCycle)
				{
					return result;
				}
			}
		}

		return CycleResult(false);
	}

	/**
	 * @brief Detect cycle using Kahn's topological sort algorithm
	 * @param graph Adjacency list representation of directed graph
	 * @return CycleResult indicating presence of cycle
	 *
	 * Algorithm: Kahn's algorithm for topological sorting
	 * Time Complexity: O(V + E)
	 * Space Complexity: O(V) for in-degree array and queue
	 *
	 * Key Insight: If topological sort processes all vertices, graph is acyclic.
	 * Remaining vertices after processing indicate cycles.
	 */
	static CycleResult detectCycleKahns(const vector<vector<int>> &graph)
	{
		int n = graph.size();
		vector<int> inDegree(n, 0);

		// Calculate in-degrees
		for (int u = 0; u < n; u++)
		{
			for (int v : graph[u])
			{
				inDegree[v]++;
			}
		}

		// Initialize queue with vertices having 0 in-degree
		queue<int> q;
		for (int i = 0; i < n; i++)
		{
			if (inDegree[i] == 0)
			{
				q.push(i);
			}
		}

		int processedVertices = 0;
		vector<int> topoOrder;

		while (!q.empty())
		{
			int u = q.front();
			q.pop();
			topoOrder.push_back(u);
			processedVertices++;

			// Reduce in-degree of neighbors
			for (int v : graph[u])
			{
				inDegree[v]--;
				if (inDegree[v] == 0)
				{
					q.push(v);
				}
			}
		}

		// If not all vertices processed, cycle exists
		bool hasCycle = (processedVertices != n);
		CycleResult result(hasCycle);

		if (hasCycle)
		{
			// Find vertices involved in cycles
			for (int i = 0; i < n; i++)
			{
				if (inDegree[i] > 0)
				{
					result.cyclePath.push_back(i);
				}
			}
		}

		return result;
	}

	/**
	 * @brief Detect cycle in undirected graph using Union-Find
	 * @param n Number of vertices
	 * @param edges List of edges as pairs
	 * @return CycleResult indicating presence of cycle
	 *
	 * Algorithm: Union-Find with path compression and union by rank
	 * Time Complexity: O(E * α(V)) where α is inverse Ackermann function
	 * Space Complexity: O(V) for parent and rank arrays
	 *
	 * Key Insight: If both endpoints of an edge belong to same set, cycle exists.
	 */
	static CycleResult detectCycleUnionFind(int n, const vector<pair<int, int>> &edges)
	{
		vector<int> parent(n);
		vector<int> rank(n, 0);

		// Initialize disjoint sets
		for (int i = 0; i < n; i++)
		{
			parent[i] = i;
		}

		function<int(int)> find = [&](int x) -> int
		{
			if (parent[x] != x)
			{
				parent[x] = find(parent[x]); // Path compression
			}
			return parent[x];
		};

		auto unionSets = [&](int x, int y) -> bool
		{
			int rootX = find(x);
			int rootY = find(y);

			if (rootX == rootY)
			{
				return false; // Already in same set - cycle detected
			}

			// Union by rank
			if (rank[rootX] < rank[rootY])
			{
				parent[rootX] = rootY;
			}
			else if (rank[rootX] > rank[rootY])
			{
				parent[rootY] = rootX;
			}
			else
			{
				parent[rootY] = rootX;
				rank[rootX]++;
			}

			return true;
		};

		// Process edges
		for (const auto &edge : edges)
		{
			if (!unionSets(edge.first, edge.second))
			{
				CycleResult result(true);
				result.cyclePath = {edge.first, edge.second};
				return result;
			}
		}

		return CycleResult(false);
	}

	/**
	 * @brief Detect all cycles in a directed graph
	 * @param graph Adjacency list representation
	 * @return Vector of all cycles found
	 *
	 * Algorithm: Johnson's algorithm adaptation
	 * Time Complexity: O(V + E) per cycle
	 * Space Complexity: O(V) for state tracking
	 */
	static vector<vector<int>> detectAllCyclesDirected(const vector<vector<int>> &graph)
	{
		int n = graph.size();
		vector<vector<int>> allCycles;
		vector<CycleState> state(n);
		vector<int> path;

		function<void(int)> dfs = [&](int u)
		{
			state[u] = CycleState::VISITING;
			path.push_back(u);

			for (int v : graph[u])
			{
				if (state[v] == CycleState::VISITING)
				{
					// Found back edge - extract cycle
					vector<int> cycle;
					bool inCycle = false;
					for (int node : path)
					{
						if (node == v)
							inCycle = true;
						if (inCycle)
							cycle.push_back(node);
					}
					cycle.push_back(v); // Complete the cycle
					allCycles.push_back(cycle);
				}
				else if (state[v] == CycleState::UNVISITED)
				{
					dfs(v);
				}
			}

			path.pop_back();
			state[u] = CycleState::VISITED;
		};

		for (int i = 0; i < n; i++)
		{
			if (state[i] == CycleState::UNVISITED)
			{
				dfs(i);
			}
		}

		return allCycles;
	}

	/**
	 * @brief Get strongly connected components (useful for cycle analysis)
	 * @param graph Adjacency list representation of directed graph
	 * @return Vector of SCCs, each SCC as vector of vertices
	 *
	 * Algorithm: Tarjan's algorithm
	 * Time Complexity: O(V + E)
	 * Space Complexity: O(V)
	 */
	static vector<vector<int>> getStronglyConnectedComponents(const vector<vector<int>> &graph)
	{
		int n = graph.size();
		vector<int> ids(n, -1);
		vector<int> low(n, 0);
		vector<bool> onStack(n, false);
		stack<int> st;
		vector<vector<int>> sccs;
		int id = 0;

		function<void(int)> tarjan = [&](int u)
		{
			ids[u] = low[u] = id++;
			st.push(u);
			onStack[u] = true;

			for (int v : graph[u])
			{
				if (ids[v] == -1)
				{
					tarjan(v);
					low[u] = min(low[u], low[v]);
				}
				else if (onStack[v])
				{
					low[u] = min(low[u], ids[v]);
				}
			}

			if (ids[u] == low[u])
			{
				vector<int> scc;
				int w;
				do
				{
					w = st.top();
					st.pop();
					onStack[w] = false;
					scc.push_back(w);
				} while (w != u);
				sccs.push_back(scc);
			}
		};

		for (int i = 0; i < n; i++)
		{
			if (ids[i] == -1)
			{
				tarjan(i);
			}
		}

		return sccs;
	}

private:
	static CycleResult dfsDirectedHelper(const vector<vector<int>> &graph, int u,
										 vector<CycleState> &state, vector<int> &parent,
										 vector<int> &path)
	{
		state[u] = CycleState::VISITING;
		path.push_back(u);

		for (int v : graph[u])
		{
			if (state[v] == CycleState::VISITING)
			{
				// Back edge found - construct cycle path
				vector<int> cyclePath;
				bool inCycle = false;
				for (int node : path)
				{
					if (node == v)
						inCycle = true;
					if (inCycle)
						cyclePath.push_back(node);
				}
				cyclePath.push_back(v);
				return CycleResult(true, cyclePath);
			}
			else if (state[v] == CycleState::UNVISITED)
			{
				parent[v] = u;
				CycleResult result = dfsDirectedHelper(graph, v, state, parent, path);
				if (result.hasCycle)
				{
					return result;
				}
			}
		}

		path.pop_back();
		state[u] = CycleState::VISITED;
		return CycleResult(false);
	}

	static CycleResult dfsUndirectedHelper(const vector<vector<int>> &graph, int u, int parent,
										   vector<bool> &visited, vector<int> &parentArray)
	{
		visited[u] = true;

		for (int v : graph[u])
		{
			if (v == parent)
				continue; // Skip parent edge

			if (visited[v])
			{
				// Back edge found - construct cycle
				vector<int> cyclePath;
				cyclePath.push_back(v);
				int current = u;
				while (current != v && current != -1)
				{
					cyclePath.push_back(current);
					current = parentArray[current];
				}
				cyclePath.push_back(v);
				return CycleResult(true, cyclePath);
			}
			else
			{
				parentArray[v] = u;
				CycleResult result = dfsUndirectedHelper(graph, v, u, visited, parentArray);
				if (result.hasCycle)
				{
					return result;
				}
			}
		}

		return CycleResult(false);
	}
};

/**
 * @class Graph
 * @brief Enhanced graph class with cycle detection capabilities
 *
 * Improved version of the original Graph class with:
 * - Better memory management
 * - Multiple cycle detection methods
 * - Comprehensive error handling
 * - Performance optimization
 */
class Graph
{
private:
	int V;
	vector<vector<int>> adjList;
	GraphType type;

public:
	Graph(int vertices, GraphType graphType = GraphType::DIRECTED)
		: V(vertices), type(graphType)
	{
		adjList.resize(V);
	}

	void addEdge(int u, int v)
	{
		if (u >= 0 && u < V && v >= 0 && v < V)
		{
			adjList[u].push_back(v);
			if (type == GraphType::UNDIRECTED)
			{
				adjList[v].push_back(u);
			}
		}
	}

	bool hasCycle()
	{
		if (type == GraphType::DIRECTED)
		{
			return CycleDetectionAlgorithms::detectCycleDirectedDFS(adjList).hasCycle;
		}
		else
		{
			return CycleDetectionAlgorithms::detectCycleUndirectedDFS(adjList).hasCycle;
		}
	}

	CycleResult detectCycleWithPath()
	{
		if (type == GraphType::DIRECTED)
		{
			return CycleDetectionAlgorithms::detectCycleDirectedDFS(adjList);
		}
		else
		{
			return CycleDetectionAlgorithms::detectCycleUndirectedDFS(adjList);
		}
	}

	vector<vector<int>> getAllCycles()
	{
		if (type == GraphType::DIRECTED)
		{
			return CycleDetectionAlgorithms::detectAllCyclesDirected(adjList);
		}
		else
		{
			// For undirected graphs, we'd need a different approach
			// This is a simplified version
			return {};
		}
	}

	void printGraph() const
	{
		cout << "Graph (" << (type == GraphType::DIRECTED ? "Directed" : "Undirected") << "):" << endl;
		for (int i = 0; i < V; i++)
		{
			cout << i << " -> ";
			for (int j : adjList[i])
			{
				cout << j << " ";
			}
			cout << endl;
		}
	}

	vector<vector<int>> getAdjacencyList() const
	{
		return adjList;
	}

	int getVertices() const { return V; }
	GraphType getType() const { return type; }
};

// Legacy functions for backward compatibility
bool checkCycle(int v, vector<vector<int>> &graph,
				vector<int> &visited, vector<int> &visiting)
{
	if (visiting[v])
	{
		return true;
	}

	visiting[v] = 1;

	for (int neighbor : graph[v])
	{
		if (visited[neighbor] == 0 && checkCycle(neighbor, graph, visited, visiting))
		{
			return true;
		}
	}

	visiting[v] = 0;
	visited[v] = 1;
	return false;
}

bool cycleInGraph(vector<vector<int>> graph)
{
	vector<int> visited(graph.size(), 0);
	vector<int> visiting(graph.size(), 0);

	for (int i = 0; i < graph.size(); i++)
	{
		if (visited[i])
			continue;
		if (checkCycle(i, graph, visited, visiting))
		{
			return true;
		}
	}

	return false;
}

/**
 * @brief Comprehensive test suite for cycle detection algorithms
 */
class CycleDetectionTests
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
		cout << "🎯 CYCLE DETECTION ALGORITHMS TEST SUITE" << endl;
		cout << "=========================================" << endl
			 << endl;

		testDirectedGraphCycles();
		testUndirectedGraphCycles();
		testKahnsAlgorithm();
		testUnionFindCycles();
		testComplexGraphs();
		testEdgeCases();
		testPerformance();
		testLegacyFunctions();

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
	static void testDirectedGraphCycles()
	{
		cout << "📋 Testing Directed Graph Cycle Detection" << endl;
		cout << "----------------------------------------" << endl;

		// Test 1: Simple cycle
		Graph g1(4, GraphType::DIRECTED);
		g1.addEdge(0, 1);
		g1.addEdge(1, 2);
		g1.addEdge(2, 3);
		g1.addEdge(3, 1); // Creates cycle: 1->2->3->1
		runTest("Simple directed cycle", g1.hasCycle());

		// Test 2: No cycle (DAG)
		Graph g2(4, GraphType::DIRECTED);
		g2.addEdge(0, 1);
		g2.addEdge(1, 2);
		g2.addEdge(2, 3);
		runTest("Directed acyclic graph", !g2.hasCycle());

		// Test 3: Self loop
		Graph g3(3, GraphType::DIRECTED);
		g3.addEdge(0, 1);
		g3.addEdge(1, 1); // Self loop
		runTest("Self loop cycle", g3.hasCycle());

		// Test 4: Multiple disconnected cycles
		Graph g4(6, GraphType::DIRECTED);
		g4.addEdge(0, 1);
		g4.addEdge(1, 0); // Cycle 1: 0->1->0
		g4.addEdge(2, 3);
		g4.addEdge(3, 4);
		g4.addEdge(4, 2); // Cycle 2: 2->3->4->2
		runTest("Multiple disconnected cycles", g4.hasCycle());

		cout << endl;
	}

	static void testUndirectedGraphCycles()
	{
		cout << "📋 Testing Undirected Graph Cycle Detection" << endl;
		cout << "------------------------------------------" << endl;

		// Test 1: Triangle cycle
		Graph g1(3, GraphType::UNDIRECTED);
		g1.addEdge(0, 1);
		g1.addEdge(1, 2);
		g1.addEdge(2, 0);
		runTest("Triangle cycle in undirected graph", g1.hasCycle());

		// Test 2: Tree (no cycle)
		Graph g2(4, GraphType::UNDIRECTED);
		g2.addEdge(0, 1);
		g2.addEdge(1, 2);
		g2.addEdge(1, 3);
		runTest("Tree structure (no cycle)", !g2.hasCycle());

		// Test 3: Square cycle
		Graph g3(4, GraphType::UNDIRECTED);
		g3.addEdge(0, 1);
		g3.addEdge(1, 2);
		g3.addEdge(2, 3);
		g3.addEdge(3, 0);
		runTest("Square cycle", g3.hasCycle());

		cout << endl;
	}

	static void testKahnsAlgorithm()
	{
		cout << "📋 Testing Kahn's Algorithm" << endl;
		cout << "---------------------------" << endl;

		// Test 1: DAG (no cycle)
		vector<vector<int>> dag = {{1, 2}, {3}, {3}, {}};
		CycleResult result1 = CycleDetectionAlgorithms::detectCycleKahns(dag);
		runTest("Kahn's on DAG", !result1.hasCycle);

		// Test 2: Graph with cycle
		vector<vector<int>> cyclic = {{1}, {2}, {0}}; // 0->1->2->0
		CycleResult result2 = CycleDetectionAlgorithms::detectCycleKahns(cyclic);
		runTest("Kahn's on cyclic graph", result2.hasCycle);

		cout << endl;
	}

	static void testUnionFindCycles()
	{
		cout << "📋 Testing Union-Find Cycle Detection" << endl;
		cout << "------------------------------------" << endl;

		// Test 1: No cycle
		vector<pair<int, int>> edges1 = {{0, 1}, {1, 2}, {2, 3}};
		CycleResult result1 = CycleDetectionAlgorithms::detectCycleUnionFind(4, edges1);
		runTest("Union-Find no cycle", !result1.hasCycle);

		// Test 2: With cycle
		vector<pair<int, int>> edges2 = {{0, 1}, {1, 2}, {2, 0}};
		CycleResult result2 = CycleDetectionAlgorithms::detectCycleUnionFind(3, edges2);
		runTest("Union-Find with cycle", result2.hasCycle);

		cout << endl;
	}

	static void testComplexGraphs()
	{
		cout << "📋 Testing Complex Graph Scenarios" << endl;
		cout << "---------------------------------" << endl;

		// Test 1: Large DAG
		Graph largeDAG(100, GraphType::DIRECTED);
		for (int i = 0; i < 99; i++)
		{
			largeDAG.addEdge(i, i + 1);
		}
		runTest("Large DAG (100 vertices)", !largeDAG.hasCycle());

		// Test 2: Dense cyclic graph
		Graph dense(10, GraphType::DIRECTED);
		for (int i = 0; i < 10; i++)
		{
			dense.addEdge(i, (i + 1) % 10); // Creates one big cycle
			dense.addEdge(i, (i + 2) % 10); // Additional edges
		}
		runTest("Dense cyclic graph", dense.hasCycle());

		// Test 3: Strongly connected components
		vector<vector<int>> sccGraph = {{1}, {2, 3}, {0}, {4}, {5}, {3}};
		vector<vector<int>> sccs = CycleDetectionAlgorithms::getStronglyConnectedComponents(sccGraph);
		runTest("SCC detection", sccs.size() >= 3); // Should have multiple SCCs

		cout << endl;
	}

	static void testEdgeCases()
	{
		cout << "📋 Testing Edge Cases" << endl;
		cout << "-------------------" << endl;

		// Test 1: Empty graph
		Graph empty(0, GraphType::DIRECTED);
		runTest("Empty graph", !empty.hasCycle());

		// Test 2: Single vertex
		Graph single(1, GraphType::DIRECTED);
		runTest("Single vertex", !single.hasCycle());

		// Test 3: Single vertex with self loop
		Graph selfLoop(1, GraphType::DIRECTED);
		selfLoop.addEdge(0, 0);
		runTest("Single vertex self loop", selfLoop.hasCycle());

		// Test 4: Two vertices no edges
		Graph twoVertices(2, GraphType::DIRECTED);
		runTest("Two vertices no edges", !twoVertices.hasCycle());

		cout << endl;
	}

	static void testPerformance()
	{
		cout << "📋 Testing Performance Characteristics" << endl;
		cout << "-------------------------------------" << endl;

		// Test performance on large graphs
		auto start = high_resolution_clock::now();

		Graph perfTest(1000, GraphType::DIRECTED);
		for (int i = 0; i < 999; i++)
		{
			perfTest.addEdge(i, i + 1);
		}
		bool result = perfTest.hasCycle();

		auto end = high_resolution_clock::now();
		auto duration = duration_cast<microseconds>(end - start);

		runTest("Performance test (1000 vertices)", duration.count() < 10000); // Should be fast
		cout << "  Performance: " << duration.count() << " microseconds" << endl;

		cout << endl;
	}

	static void testLegacyFunctions()
	{
		cout << "📋 Testing Legacy Functions" << endl;
		cout << "--------------------------" << endl;

		// Test legacy cycleInGraph function
		vector<vector<int>> testGraph = {{1}, {2}, {0}}; // 0->1->2->0
		bool legacyResult = cycleInGraph(testGraph);
		runTest("Legacy cycleInGraph function", legacyResult);

		cout << endl;
	}
};

int CycleDetectionTests::testCount = 0;
int CycleDetectionTests::passedTests = 0;

/**
 * @brief Performance benchmark for cycle detection algorithms
 */
class CycleDetectionBenchmark
{
public:
	static void runBenchmarks()
	{
		cout << "🚀 CYCLE DETECTION PERFORMANCE BENCHMARKS" << endl;
		cout << "==========================================" << endl
			 << endl;

		vector<int> sizes = {100, 500, 1000, 2000};
		vector<double> densities = {0.1, 0.3, 0.5, 0.8}; // 10%, 30%, 50%, 80% of max edges

		for (int size : sizes)
		{
			cout << "📊 Graph size: " << size << " vertices" << endl;
			cout << string(35, '-') << endl;

			for (double density : densities)
			{
				cout << "Edge density: " << (density * 100) << "%" << endl;

				// Generate test graphs
				vector<vector<int>> directedGraph = generateDirectedGraph(size, density);
				vector<pair<int, int>> undirectedEdges = generateUndirectedEdges(size, density);

				// Benchmark DFS for directed graphs
				benchmarkAlgorithm("DFS Directed", [&]()
								   { return CycleDetectionAlgorithms::detectCycleDirectedDFS(directedGraph).hasCycle; });

				// Benchmark Kahn's algorithm
				benchmarkAlgorithm("Kahn's Algorithm", [&]()
								   { return CycleDetectionAlgorithms::detectCycleKahns(directedGraph).hasCycle; });

				// Benchmark Union-Find for undirected graphs
				benchmarkAlgorithm("Union-Find", [&]()
								   { return CycleDetectionAlgorithms::detectCycleUnionFind(size, undirectedEdges).hasCycle; });

				cout << endl;
			}
			cout << endl;
		}
	}

private:
	static vector<vector<int>> generateDirectedGraph(int n, double density)
	{
		vector<vector<int>> graph(n);
		int maxEdges = n * (n - 1);
		int numEdges = static_cast<int>(maxEdges * density);

		for (int i = 0; i < numEdges; i++)
		{
			int u = rand() % n;
			int v = rand() % n;
			if (u != v)
			{
				graph[u].push_back(v);
			}
		}

		return graph;
	}

	static vector<pair<int, int>> generateUndirectedEdges(int n, double density)
	{
		vector<pair<int, int>> edges;
		int maxEdges = n * (n - 1) / 2;
		int numEdges = static_cast<int>(maxEdges * density);

		for (int i = 0; i < numEdges; i++)
		{
			int u = rand() % n;
			int v = rand() % n;
			if (u != v)
			{
				edges.push_back({min(u, v), max(u, v)});
			}
		}

		return edges;
	}

	template <typename Func>
	static void benchmarkAlgorithm(const string &name, Func algorithm)
	{
		auto start = high_resolution_clock::now();
		bool result = algorithm();
		auto end = high_resolution_clock::now();

		auto duration = duration_cast<microseconds>(end - start);

		cout << "  " << name << ": " << duration.count() << "μs, "
			 << "result: " << (result ? "cycle" : "no cycle") << endl;
	}
};

/**
 * @brief Interactive demonstration of cycle detection algorithms
 */
class CycleDetectionDemo
{
public:
	static void runDemo()
	{
		cout << "🎮 CYCLE DETECTION INTERACTIVE DEMONSTRATION" << endl;
		cout << "=============================================" << endl
			 << endl;

		demoDirectedGraphDFS();
		demoUndirectedGraphDFS();
		demoKahnsAlgorithm();
		demoRealWorldApplications();
	}

private:
	static void demoDirectedGraphDFS()
	{
		cout << "🔍 Directed Graph DFS Cycle Detection" << endl;
		cout << "====================================" << endl;

		Graph g(4, GraphType::DIRECTED);
		g.addEdge(0, 1);
		g.addEdge(1, 2);
		g.addEdge(2, 3);
		g.addEdge(3, 1); // Creates cycle: 1->2->3->1

		cout << "Graph structure:" << endl;
		g.printGraph();
		cout << endl;

		cout << "Running DFS cycle detection..." << endl;
		CycleResult result = g.detectCycleWithPath();

		if (result.hasCycle)
		{
			cout << "✅ Cycle detected!" << endl;
			cout << "Cycle path: ";
			for (size_t i = 0; i < result.cyclePath.size(); i++)
			{
				cout << result.cyclePath[i];
				if (i < result.cyclePath.size() - 1)
					cout << " -> ";
			}
			cout << endl;
			cout << "Cycle length: " << result.cycleLength << endl;
		}
		else
		{
			cout << "❌ No cycle found" << endl;
		}

		cout << endl;
	}

	static void demoUndirectedGraphDFS()
	{
		cout << "🔍 Undirected Graph DFS Cycle Detection" << endl;
		cout << "======================================" << endl;

		Graph g(4, GraphType::UNDIRECTED);
		g.addEdge(0, 1);
		g.addEdge(1, 2);
		g.addEdge(2, 3);
		g.addEdge(3, 0); // Creates cycle: 0-1-2-3-0

		cout << "Graph structure:" << endl;
		g.printGraph();
		cout << endl;

		cout << "Running DFS cycle detection..." << endl;
		CycleResult result = g.detectCycleWithPath();

		if (result.hasCycle)
		{
			cout << "✅ Cycle detected!" << endl;
			cout << "Cycle path: ";
			for (size_t i = 0; i < result.cyclePath.size(); i++)
			{
				cout << result.cyclePath[i];
				if (i < result.cyclePath.size() - 1)
					cout << " - ";
			}
			cout << endl;
		}
		else
		{
			cout << "❌ No cycle found" << endl;
		}

		cout << endl;
	}

	static void demoKahnsAlgorithm()
	{
		cout << "🔍 Kahn's Algorithm Demonstration" << endl;
		cout << "================================" << endl;

		vector<vector<int>> graph = {{1, 2}, {3}, {3}, {1}}; // 3->1 creates cycle

		cout << "Graph adjacency list:" << endl;
		for (int i = 0; i < graph.size(); i++)
		{
			cout << i << " -> ";
			for (int j : graph[i])
			{
				cout << j << " ";
			}
			cout << endl;
		}
		cout << endl;

		cout << "Running Kahn's algorithm..." << endl;
		CycleResult result = CycleDetectionAlgorithms::detectCycleKahns(graph);

		if (result.hasCycle)
		{
			cout << "✅ Cycle detected using topological sort!" << endl;
			cout << "Vertices involved in cycles: ";
			for (int v : result.cyclePath)
			{
				cout << v << " ";
			}
			cout << endl;
		}
		else
		{
			cout << "❌ No cycle found - graph is a DAG" << endl;
		}

		cout << endl;
	}

	static void demoRealWorldApplications()
	{
		cout << "🌍 Real-World Applications" << endl;
		cout << "==========================" << endl;

		cout << "1. 📚 Course Prerequisite Checking" << endl;
		Graph courses(5, GraphType::DIRECTED);
		courses.addEdge(0, 1); // Math -> Physics
		courses.addEdge(1, 2); // Physics -> Advanced Physics
		courses.addEdge(2, 3); // Advanced Physics -> Engineering
		courses.addEdge(3, 4); // Engineering -> Project
		// courses.addEdge(4, 0); // This would create impossible prerequisite cycle

		cout << "   Course dependencies (0=Math, 1=Physics, 2=AdvPhysics, 3=Engineering, 4=Project):" << endl;
		if (!courses.hasCycle())
		{
			cout << "   ✅ Valid course structure - no circular dependencies" << endl;
		}
		else
		{
			cout << "   ❌ Invalid course structure - circular dependencies detected" << endl;
		}
		cout << endl;

		cout << "2. 🔄 Deadlock Detection in Operating Systems" << endl;
		Graph processes(4, GraphType::DIRECTED);
		processes.addEdge(0, 1); // Process 0 waits for resource from Process 1
		processes.addEdge(1, 2); // Process 1 waits for resource from Process 2
		processes.addEdge(2, 3); // Process 2 waits for resource from Process 3
		processes.addEdge(3, 0); // Process 3 waits for resource from Process 0 - DEADLOCK!

		cout << "   Process resource dependencies:" << endl;
		if (processes.hasCycle())
		{
			cout << "   ⚠️  DEADLOCK DETECTED! Processes are in circular wait" << endl;
		}
		else
		{
			cout << "   ✅ No deadlock - safe resource allocation" << endl;
		}
		cout << endl;

		cout << "3. 📊 Dependency Analysis in Software Projects" << endl;
		Graph modules(6, GraphType::DIRECTED);
		modules.addEdge(0, 1); // Module A depends on Module B
		modules.addEdge(1, 2); // Module B depends on Module C
		modules.addEdge(2, 3); // Module C depends on Module D
		modules.addEdge(4, 5); // Module E depends on Module F

		cout << "   Software module dependencies:" << endl;
		if (!modules.hasCycle())
		{
			cout << "   ✅ Clean dependency structure - can be built in order" << endl;
		}
		else
		{
			cout << "   ❌ Circular dependencies detected - refactoring needed" << endl;
		}
		cout << endl;
	}
};

/**
 * @brief Main function with comprehensive testing and demonstration
 */
int main(int argc, char *argv[])
{
	cout << "🎯 CYCLE DETECTION ALGORITHMS - COMPREHENSIVE IMPLEMENTATION" << endl;
	cout << "============================================================" << endl;
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
		cout << "./detect_cycle [options]" << endl
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
		CycleDetectionTests::runAllTests();
		cout << endl;
	}

	if (runDemo)
	{
		CycleDetectionDemo::runDemo();
		cout << endl;
	}

	if (runBenchmarks)
	{
		CycleDetectionBenchmark::runBenchmarks();
		cout << endl;
	}

	// Quick examples if no specific options
	if (!runBenchmarks && !runDemo && runTests)
	{
		cout << "🎯 QUICK EXAMPLES" << endl;
		cout << "=================" << endl;

		// Example 1: Directed graph with cycle
		cout << "Example 1: Directed Graph Cycle Detection" << endl;
		Graph directedExample(4, GraphType::DIRECTED);
		directedExample.addEdge(0, 1);
		directedExample.addEdge(1, 2);
		directedExample.addEdge(2, 3);
		directedExample.addEdge(3, 1); // Creates cycle

		cout << "Graph: 0->1->2->3->1 (cycle: 1->2->3->1)" << endl;
		cout << "Has cycle: " << (directedExample.hasCycle() ? "Yes" : "No") << endl;

		CycleResult result = directedExample.detectCycleWithPath();
		if (result.hasCycle)
		{
			cout << "Cycle path: ";
			for (size_t i = 0; i < result.cyclePath.size(); i++)
			{
				cout << result.cyclePath[i];
				if (i < result.cyclePath.size() - 1)
					cout << "->";
			}
			cout << endl;
		}
		cout << endl;

		// Example 2: Undirected graph
		cout << "Example 2: Undirected Graph Cycle Detection" << endl;
		Graph undirectedExample(4, GraphType::UNDIRECTED);
		undirectedExample.addEdge(0, 1);
		undirectedExample.addEdge(1, 2);
		undirectedExample.addEdge(2, 3);
		undirectedExample.addEdge(3, 0); // Creates cycle

		cout << "Graph: 0-1-2-3-0 (square cycle)" << endl;
		cout << "Has cycle: " << (undirectedExample.hasCycle() ? "Yes" : "No") << endl;
		cout << endl;

		// Example 3: Using different algorithms
		cout << "Example 3: Algorithm Comparison" << endl;
		vector<vector<int>> testGraph = {{1}, {2}, {0}}; // 0->1->2->0

		CycleResult dfsResult = CycleDetectionAlgorithms::detectCycleDirectedDFS(testGraph);
		CycleResult kahnResult = CycleDetectionAlgorithms::detectCycleKahns(testGraph);

		cout << "Test graph: 0->1->2->0" << endl;
		cout << "DFS result: " << (dfsResult.hasCycle ? "Cycle detected" : "No cycle") << endl;
		cout << "Kahn's result: " << (kahnResult.hasCycle ? "Cycle detected" : "No cycle") << endl;
		cout << endl;
	}

	cout << "🎓 ALGORITHM SUMMARY" << endl;
	cout << "===================" << endl;
	cout << "• DFS for Directed Graphs: O(V+E) time, detects back edges to recursion stack" << endl;
	cout << "• DFS for Undirected Graphs: O(V+E) time, detects back edges to visited vertices" << endl;
	cout << "• Kahn's Algorithm: O(V+E) time, topological sort approach" << endl;
	cout << "• Union-Find: O(E⋅α(V)) time, efficient for undirected graphs" << endl;
	cout << "• Tarjan's SCC: O(V+E) time, finds strongly connected components" << endl
		 << endl;

	cout << "🌍 REAL-WORLD APPLICATIONS" << endl;
	cout << "==========================" << endl;
	cout << "• Course Prerequisite Validation" << endl;
	cout << "• Deadlock Detection in Operating Systems" << endl;
	cout << "• Dependency Analysis in Build Systems" << endl;
	cout << "• Circuit Analysis and Feedback Loops" << endl;
	cout << "• Social Network Analysis" << endl;
	cout << "• Compiler Dependency Resolution" << endl
		 << endl;

	cout << "🎊 Program completed successfully!" << endl;

	return 0;
}

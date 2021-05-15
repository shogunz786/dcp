#include <vector>
#include <stack>
#include <set>
#include <iostream>
using namespace std;

class Graph{
	public:
	  unordered_map<int, vector<int>> adjList;
	  Graph():adjList({}){}
	  void addEdge(vector<int> &v){
			if(adjList.find(v[0])==adjList.end()){
				adjList.insert({v[0],{v[1]}});
			}else{
				adjList[v[0]].push_back(v[1]);
			}
		}
	  void populate(vector<vector<int>> &vv){
			for(vector v: vv){
				addEdge(v);
			}
		}
	bool topoUtil(int b, unordered_set<int> &visited, stack<int> &st, unordered_set<int>& visiting){
		if(visited.count(b))
			return false;
		if(visiting.count(b))
			return true;
		visiting.insert(b);
		if(adjList.find(b)!=adjList.end()){
			for(int e:adjList[b]){
					cout<<b<<"->"<<e<<endl;
					if(topoUtil(e, visited, st, visiting)){
						return true;
					}
			}
		}
		visiting.erase(b);
		visited.insert(b);
		cout<<b<<"<<"<<endl;
		st.push(b);
		return false;
	}
};
//O(j+d) time and O(j+d) space j - jobs and d - dependencies
vector<int> topologicalSort(vector<int> jobs, vector<vector<int>> deps) {
	Graph g;
	vector<int> res;
	unordered_set<int> visited;
	unordered_set<int> visiting;
	stack<int> st;
	g.populate(deps);
	for(int i: jobs) {
			if(g.topoUtil(i, visited, st, visiting)){
				return {};
			}
	}
	while(!st.empty()){
	  res.push_back(st.top());
		st.pop();
	}
  return res;
}

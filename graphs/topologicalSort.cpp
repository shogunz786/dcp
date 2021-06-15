/*
 * given jobs 1,2,3,4
 * dependencies for jobs 1-2,1-3,3-2,4-2,4-5 second jod depends on first
 * return list of jovs with valid order of job completion. 1,4,3,2 or 4,1,3,2
 */
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

//Use inDegree tracking
using namespace std;

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <vector>

class TopologicalSort {
 public:
  static vector<int> sort(int vertices, const vector<vector<int>>& edges) {
    vector<int> sortedOrder;
    vector<int> inDegree(vertices, 0);
    queue<int> sources;
    unordered_map<int, vector<int>>  mp;
    stack<int> res;
    for(auto v: edges){
      inDegree[v[1]]++;//keep track of incoming connections
      if(mp.find(v[0])==mp.end()){
        mp.insert({v[0],{}});
      }
      mp[v[0]].push_back(v[1]);
    }
    for(int i=0; i<vertices; i++){
       if(inDegree[i]==0){//use the vertex with zero incoming            
         sources.push(i);//connections as starting vertices to explore paths
       }
    }
    while(sources.size()){
      int v = sources.front();
      sources.pop();
      sortedOrder.push_back(v);
      if(mp.find(v)!=mp.end()){
         for(auto a: mp[v]){
           inDegree[a]--;//deduct incoming path as exploration is complete
           if(inDegree[a]==0){
             sources.push(a);
           }
         }
      }
    }
    if(sortedOrder.size()!=vertices)//if there are cycles return empty array
        return vector<int>();
    return sortedOrder;
  }

};

int main(int argc, char* argv[]) {
  vector<int> result =
      TopologicalSort::sort(4, vector<vector<int>>{vector<int>{3, 2}, vector<int>{3, 0},
                                                   vector<int>{2, 0}, vector<int>{2, 1}});
  for (auto num : result) {
    cout << num << " ";
  }
  cout << endl;

  result = TopologicalSort::sort(
      5, vector<vector<int>>{vector<int>{4, 2}, vector<int>{4, 3}, vector<int>{2, 0},
                             vector<int>{2, 1}, vector<int>{3, 1}});
  for (auto num : result) {
    cout << num << " ";
  }
  cout << endl;

  result = TopologicalSort::sort(
      7, vector<vector<int>>{vector<int>{6, 4}, vector<int>{6, 2}, vector<int>{5, 3},
                             vector<int>{5, 4}, vector<int>{3, 0}, vector<int>{3, 1},
                             vector<int>{3, 2}, vector<int>{4, 1}});
  for (auto num : result) {
    cout << num << " ";
  }
  cout << endl;
}


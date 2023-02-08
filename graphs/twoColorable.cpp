#include <vector>
#include <stack>
using namespace std;

//O(v+e) time and O(v) space
//v - vertices. e - edges
bool twoColorable(vector<vector<int>> edges) {
  //each index in edges is a vertex
  //each row in edges has vertex siblings
  vector<int> colors(edges.size(),0);
  stack<int> vertices;
  //first vertex color is set to 1
  colors[0]=1;

  //add vertex 0 to stack
  vertices.push(0);

  //while more vertices to process
  while(vertices.size()){
    //get the latest vertex being processed
    int node = vertices.top();
    //remove from stack as it will be processed
    vertices.pop();
    //process siblings of the vertex
    for(int conn: edges[node]){
      //color is not set for the vertex
      if(colors[conn]==0){
        //if color not set, then set 1 else set 2.
        colors[conn] = colors[node] == 1 ? 2 :1;
        //add sibling to vertices to process
        vertices.push(conn);
      }else if(colors[conn] == colors[node]){
        //if sibling has same color as the vertex return false
        return false;
      }
    }
  }

/*
There are ‘N’ tasks, labeled from ‘0’ to ‘N-1’. Each task can have some prerequisite tasks which need to be completed 
before it can be scheduled. Given the number of tasks and a list of prerequisite pairs, find out if it is possible to schedule all the tasks.

Example 1:

Input: Tasks=3, Prerequisites=[0, 1], [1, 2]
Output: true
Explanation: To execute task '1', task '0' needs to finish first. Similarly, task '1' needs to finish 
before '2' can be scheduled. A possible sceduling of tasks is: [0, 1, 2] 
Example 2:

Input: Tasks=3, Prerequisites=[0, 1], [1, 2], [2, 0]
Output: false
Explanation: The tasks have cyclic dependency, therefore they cannot be sceduled.
Example 3:

Input: Tasks=6, Prerequisites=[2, 5], [0, 5], [0, 4], [1, 4], [3, 2], [1, 3]
Output: true
Explanation: A possible sceduling of tasks is: [0 1 4 3 2 5] 
*/

using namespace std;

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

class TaskScheduling {
 public:
 //O(V+E) time and O(V+E) space V - vertices and E - Edges
  static bool isSchedulingPossible(int tasks, const vector<vector<int>>& prerequisites) {
    vector<int> inDegree(tasks,0);
    queue<int> sources;
    vector<int> sortedOrder;
    unordered_map<int,vector<int>> mp;

    for(auto a: prerequisites){
      inDegree[a[1]]++;
      if(mp.find(a[0])==mp.end()){
        mp.insert({a[0],{}});
      }
      mp[a[0]].push_back(a[1]);
    }
    for(int i=0;i<inDegree.size(); i++){
      if(inDegree[i]==0){
        sources.push(i);
      }
    }
    while(!sources.empty()){
      int s = sources.front();
      sources.pop();
      sortedOrder.push_back(s);
      if(mp.find(s)!=mp.end()){
        for(auto a: mp[s]){
          inDegree[a]--;
          if(inDegree[a]==0){
            sources.push(a);
          }
        }
      }
    }

    return sortedOrder.size()==tasks;
  }
};

int main(int argc, char* argv[]) {
  bool result = TaskScheduling::isSchedulingPossible(
      3, vector<vector<int>>{vector<int>{0, 1}, vector<int>{1, 2}});
  cout << "Tasks execution possible: " << result << endl;

  result = TaskScheduling::isSchedulingPossible(
      3, vector<vector<int>>{vector<int>{0, 1}, vector<int>{1, 2}, vector<int>{2, 0}});
  cout << "Tasks execution possible: " << result << endl;

  result = TaskScheduling::isSchedulingPossible(
      6, vector<vector<int>>{vector<int>{2, 5}, vector<int>{0, 5}, vector<int>{0, 4},
                             vector<int>{1, 4}, vector<int>{3, 2}, vector<int>{1, 3}});
  cout << "Tasks execution possible: " << result << endl;
}

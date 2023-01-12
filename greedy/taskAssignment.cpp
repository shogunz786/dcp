#include <vector>
using namespace std;

vector<vector<int>> taskAssignment(int k, vector<int> tasks) {
 
  vector<int> sortedTasks = tasks;
  //sort tasks
  sort(sortedTasks.begin(), sortedTasks.end());

  //keep track indexes
  unordered_map<int,vector<int>> mp;
  for(int i=0; i<tasks.size(); i++){
    if(mp.find(tasks[i])!=mp.end()){
      mp[tasks[i]].push_back(i);
    }else{
      mp[tasks[i]]=vector<int>{i};
    }
  }

  //assign a left and a right task to get least overall time as it
  //averages out
  int left =0;
  int right = tasks.size()-1;
  vector<vector<int>> res;
  while(left<right){
    int leftIdx = mp[sortedTasks[left]].back();
    int rightIdx = mp[sortedTasks[right]].back();
    res.push_back(vector<int>{leftIdx, rightIdx});
    mp[sortedTasks[left]].pop_back();
    mp[sortedTasks[right]].pop_back();
    left++;
    right--;
  }
  
  return res;
}

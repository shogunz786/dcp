#include <vector>
using namespace std;

//O(nc) time and O(nc) space
vector<vector<int>> getKSItems(vector<vector<int>> &ksValues,
vector<vector<int>> &items, int weight, int capacity){
  int c = capacity;
  int i=items.size();
  //backtracking from bottom right
  vector<vector<int>> res = {{},{}};
  while(i>0){
    //if current item is included
    if(ksValues[i][c]!=ksValues[i-1][c]){
      c-=items[i-1][1];
      res[1].insert(res[1].begin(),i-1);
    }
    i--;
    if(c==0){
      break;
    }
  }
  //add max weight possible
  res[0].push_back(weight);
  return res;
}

vector<vector<int>> knapsackProblem(vector<vector<int>> items, int capacity){
  //first row is capacity zero
  vector<vector<int>> ksValues(items.size()+1, vector<int>(capacity+1,0));
  for(int i=1; i<items.size()+1; i++){
    int curWt = items[i-1][1];
    int curValue = items[i-1][0];
    for(int c=0; c<capacity+1; c++){
      if(curWt > c ){
        //wt is greater than current capacity
        //so use same max value at that capacity
        ksValues[i][c]=ksValues[i-1][c];
      }else{
        //max of current max value at the current capacity
        //or use max value when removed current weight 
        //from current max capacity plus the current value
        ksValues[i][c]=max(ksValues[i-1][c],
          ksValues[i-1][c-curWt]+curValue);
      }
    }
  }
  //backtrack to find the items picked
  return getKSItems(ksValues, items, ksValues[items.size()][capacity], capacity);
}

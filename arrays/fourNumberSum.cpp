#include <vector>
using namespace std;

//[0..k]i[i+1..n]
vector<vector<int>> fourNumberSum(vector<int> array, int targetSum) {
  unordered_map<int, vector<vector<int>>> mp;
  vector<vector<int>> res;
  for(int i=0; i<array.size()-1; i++){
    //check if the remaining sum is present
    for(int j=i+1; j<array.size(); j++){
      int diff = targetSum - (array[i]+array[j]);
      //see if the diff has a pair
      if(mp.find(diff)!=mp.end()){
        //make result from all the diff pairs
        for(auto a: mp[diff]){
          //add the pair for diff to the remaining part 
          a.push_back(array[i]);
          a.push_back(array[j]);
          res.push_back(a);
        }
      }
    }
    for(int k=0; k<i; k++){
      int sum = array[i]+array[k];
      //keep track all sum pairs
      if(mp.find(sum)==mp.end()){
        mp[sum]={{array[i],array[k]}};
      }else{
        mp[sum].push_back({array[i],array[k]});
      }
    }
  }
    
  return res;
}

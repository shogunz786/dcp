#include <vector>
using namespace std;

//O(n) time and o(n) space
vector<int> largestRange(vector<int> array) {
  vector<int> bestRange;
  int longestLen = 0;
  unordered_map<int, bool> nums;

  //create a map of unique elements
  for(int n: array){
    nums[n]=true;
  }

  for(int n: array){
    //ignore processed elements in case of duplicates
    if(!nums[n]){
      continue;
    }
    //mark processed elements
    nums[n]=false;
    int curLen = 1;
    int left = n-1;
    int right = n+1;
    //go left from n and calculate len till out of range
    while(nums.find(left)!=nums.end()){
      nums[left]=false;
      curLen++;
      left--;
    }
    //go right from n and calcluate len till out of range
    while(nums.find(right)!=nums.end()){
      nums[right]=false;
      curLen++;
      right++;
    }
    //keep track of longest length
    if(curLen > longestLen){
      longestLen = curLen;
      //keep track of range
      bestRange = {left+1, right-1};
    }
  }
  return bestRange;
}

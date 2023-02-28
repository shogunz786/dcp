using namespace std;

//O(n) space and O(n) time
bool zeroSumSubarray(vector<int> nums) {
  unordered_set<int> st = {0};
  int currentSum = 0;
  for(auto num: nums){
    currentSum += num;
    if(st.find(currentSum)!=st.end()){
      return true;
    }
    st.insert(currentSum);
  }
  return false;
}

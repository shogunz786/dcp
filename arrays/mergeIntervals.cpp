/*
  Given non sorted array of intervals
	return merged array of intervals.
	input:
	"intervals": [
    [1, 2],
    [3, 5],
    [4, 7],
    [6, 8],
    [9, 10]
  ]
	output:
	[
  [1, 2],
  [3, 8],
  [9, 10]
]
*/
#include <vector>
using namespace std;

bool compare(vector<int> a, vector<int> b){
	return a[0]<b[0];
}

//O(nlogn) space and O(n) space
vector<vector<int>> mergeOverlappingIntervals(vector<vector<int>> intervals) {
	vector<vector<int>> res;
	sort(intervals.begin(),intervals.end(),compare);
	res.push_back(intervals[0]);
	
	for(int i=1;i<intervals.size(); i++){
	   if(intervals[i][0]<=res[res.size()-1][1])	
		 {
			 res[res.size()-1][1]=max(intervals[i][1],res[res.size()-1][1]);
		 }else{
			 res.push_back(intervals[i]);
		 }
	}
  return res;
}

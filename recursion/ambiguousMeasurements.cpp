/*
Given low and high values of measuring cups
[
  [200, 210],
  [450, 465],
  [800, 850]
]
compute true or false if the
[2100,2300] can be computed by using lows to compute low and highs to compute high and using one cup at a time.

*/
#include <vector>
using namespace std;

bool helper(vector<vector<int>> measuringCups, int low,
                           int high, int sumLow, int sumHigh, int i){
	//allow to loop around other cups
	i=i%measuringCups.size();
	//compute new sums
	sumLow+=measuringCups[i][0];
	sumHigh+=measuringCups[i][1];
	//return false if either sum goes past high
	if(sumHigh>=high || sumLow>=high){
		return false;
	}
	if(sumHigh<high and low<sumLow){
		return true;
	}
	//consider same cup again and pick new cup next time
	return helper(measuringCups, low, high, sumLow, sumHigh,i) ||
	       helper(measuringCups, low, high, sumLow, sumHigh,i+1);
}

bool ambiguousMeasurements(vector<vector<int>> measuringCups, int low,
                           int high) {
	//recurse with each cup
	for(int i=0; i<measuringCups.size(); i++){
		if(helper(measuringCups,low,high,0,0,i))
			return true;
	}
	return false;
}


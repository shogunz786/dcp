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

//O(2^n) time and O(n) space
bool helper1(vector<vector<int>> measuringCups, int low,
                           int high, int sumLow, int sumHigh){
	bool result=false;
	//return false if either sum goes past high
	if(sumHigh>=high || sumLow>=high){
	   return false;
	}
	if(sumHigh<high && sumLow>low){
	   return true;
	}
	for(int i=0; i<measuringCups.size(); i++){
	   //allow to loop around other cups
	   //compute new sums
	   sumLow+=measuringCups[i][0];
	   sumHigh+=measuringCups[i][1];
	   //consider same cup again and pick new cup next time
	   result=helper1(measuringCups, low, high, sumLow, sumHigh);
	   if(result){
	      break; 
           }
	}
	return result;
}

bool ambiguousMeasurements1(vector<vector<int>> measuringCups, int low,
                           int high) {
	return helper1(measuringCups,low,high,0,0);
}


bool helperRec1(vector<vector<int>> measuringCups, int low,
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
	return helperRec1(measuringCups, low, high, sumLow, sumHigh,i) ||
	       helperRec1(measuringCups, low, high, sumLow, sumHigh,i+1);
}

bool ambiguousMeasurementsRec1(vector<vector<int>> measuringCups, int low,
                           int high) {
	//recurse with each cup
	for(int i=0; i<measuringCups.size(); i++){
		if(helperRec1(measuringCups,low,high,0,0,i))
			return true;
	}
	return false;
}

//memoize solution
//O(low*high*n) time and O(low*high) space
bool helper(vector<vector<int>> measuringCups, int low, int high,
						int sumLow, int sumHigh,
						unordered_map<string,bool> &mp){
	int result=false;
	string key=to_string(sumLow)+"-"+to_string(sumHigh);
	//check memoized results
	if(mp.find(key)!=mp.end()){
		return mp[key];
	}

	//return false if either sum goes past high
	if(sumHigh > high){
	   return false;
	}
	//                  low:high
	//		    /   |   \
        //               cup1	cup2 cup3
	//start trying from certain sum and try all the cups
	//and capture the result
	for(auto cup: measuringCups){
	   //allow to loop around other cups
	   //compute new sums
		 int nLow=sumLow+cup[0];
		 int nHigh=sumHigh+cup[1];
		 //if valid range return true
		 if(nLow>=low && nHigh<=high){
	      result=true;
			  break;
	   }
	   //consider same cup again and pick new cup next time
	   result=helper(measuringCups, low, high, nLow, nHigh, mp);
		 if(result){
			 break;
		 }
	}
	mp[key]=result;
	return result;
}

bool ambiguousMeasurements(vector<vector<int>> measuringCups, int low,
                           int high) {
	unordered_map<string,bool> mp;
	return helper(measuringCups,low,high,0,0,mp);
}


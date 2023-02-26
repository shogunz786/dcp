/*
 Given set of coins find the minimum denomination
 that can't be made with the coins.
 input:"coins": [5, 7, 1, 1, 2, 3, 22]
 output:20
 */
 
#include <vector>
using namespace std;

//O(nlogn) time and space O(1)
int nonConstructibleChange(vector<int> coins) {
	sort(coins.begin(),coins.end());
	int currentMax = 0;
	for(auto c: coins){
		//current coin + 1 > sum of total change
		//subset of coins so far can make change upto currentMax
		if(c>currentMax+1){
			return currentMax+1;
		}
		currentMax+=c;
	}
	return currentMax+1;
}

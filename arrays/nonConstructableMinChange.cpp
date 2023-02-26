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
		//current coin > sum of total change in bag + 1
		//subset of coins in bag so far can make change upto currentMax is proven by math
		if(c>currentMax+1){
			//if current coin > sum of total change + 1, then
			//that means the bags in the coins together with current
			//coin cannot make coins up to currrentMax + 1
			return currentMax+1;
		}
		//add coin to bag
		currentMax+=c;
	}
	return currentMax+1;
}

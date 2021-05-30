/*
max profits with k transactions
prices of stocks on different days
[5, 11, 3, 50, 60, 90]
can make maximum of t=2 transactions

find max profit that can be made.
result=93

Let profit[t][i] represent maximum profit using at most t transactions
up to day i (including day i). Then the relation is:
profit[t][i] = max(profit[t][i-1], 
for all j in range [0, i-1] 
    max(price[i] – price[j] + profit[t-1][j])))
                
 
profit[t][i] will be maximum of –
1. profit[t][i-1] which represents not doing any transaction on the ith day.
2. Maximum profit gained by selling on ith day. In order to sell 
   shares on ith day, we need to purchase it on any one
   of [0, i – 1] days. If we buy shares on jth day and sell it on 
   ith day, max profit will be price[i] – price[j] + profit[t-1][j] where
   j varies from 0 to i-1. Here profit[t-1][j] is best we could have
   done with one less transaction till jth day.

*/
#include <vector>
using namespace std;

//O(nk) time and O(nk) space
int maxProfitWithKTransactionsDP1(vector<int> prices, int k) {
	if(k==0 || prices.size()==0){
		return 0;
	}
	vector<vector<int>> profits(k+1,vector<int>(prices.size(),0));
	for(int t=1; t<=k; t++ ){
		int maxsofar = INT_MIN;
		//at each day
		for(int d=1; d<prices.size(); d++){
			//profits[t][d]=max(profits[t][d-1],price[i]+max(profits[t-1][j]-price[j]))
			//using kadane's algorithm to find max profit so far
			maxsofar=max(maxsofar,profits[t-1][d-1]-prices[d-1]);
			profits[t][d]=max(profits[t][d-1],maxsofar+prices[d]);
		}
	}
  return profits[k][prices.size()-1];
}

//O(nk) time and O(n) space
int maxProfitWithKTransactions(vector<int> prices, int k) {
	if(k==0 || prices.size()==0){
		return 0;
	}
	vector<int> evenprofits(prices.size(),0);
	vector<int> oddprofits(prices.size(),0);
	for(int t=1; t<=k; t++ ){
		int maxsofar = INT_MIN;
		//at each day
		for(int d=1; d<prices.size(); d++){
			//profits[t][d]=max(profits[t][d-1],price[i]+max(profits[t-1][j]-price[j]))
			//using kadane's algorithm to find max profit so far
			//to save space use just prev and current rows
			if(t%2){
			   maxsofar=max(maxsofar,evenprofits[d-1]-prices[d-1]);
			   oddprofits[d]=max(oddprofits[d-1],maxsofar+prices[d]);
			}else{
			   maxsofar=max(maxsofar,oddprofits[d-1]-prices[d-1]);
			   evenprofits[d]=max(evenprofits[d-1],maxsofar+prices[d]);
			}
		}
	}
  return k%2?oddprofits[prices.size()-1]:evenprofits[prices.size()-1];
}

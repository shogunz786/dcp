/*
 * longest common subsequence of two strings, the characters does not have to be adjacent, but need to be in order.
 * Single charcter in a string and string itself are both valid subsequences of the string.
 * str1= "ZXVVYZW"
 * str2= "XKYKZPW"
 *
 * output: X, Y, Z, W
 */
#include <vector>
using namespace std;

vector<char> buildSeq(vector<vector<int>> &dp, string &str1, string &str2){
	vector<char> subseq;
	int i=str1.length(), j=str2.length();
	while(i>0 && j>0){
	  if(dp[i][j]==dp[i-1][j]){
			i--;
		}else if(dp[i][j]==dp[i][j-1]){
			j--;
		}else{
			subseq.insert(subseq.begin(),str2[j-1]);
			i--;
			j--;
		}	
	}
	return subseq;
}

//O(nm) time and O(nm) space
vector<char> longestCommonSubsequence(string str1, string str2) {
	vector<vector<int>> dp(str1.length()+1,vector<int>(str2.length()+1,0));
	for(int i=1; i<str1.length()+1; i++){
		for(int j=1; j<str2.length()+1; j++){
			if(str1[i-1]==str2[j-1]){
				dp[i][j]=dp[i-1][j-1]+1;
			}else{
				dp[i][j]=max(dp[i][j-1],dp[i-1][j]);
			}
		}
	}
  return buildSeq(dp, str1, str2);
}

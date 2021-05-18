/*
 *input:{
  "competitions": [
    ["HTML", "C#"],
    ["C#", "Python"],
    ["Python", "HTML"]
  ],
  "results": [0, 0, 1]
}
result[0]=0 means in first game HTML vs C# ..C# the away team won
if result is 1 home team won.
*/

#include <vector>
using namespace std;
//keep track of the winner with 1 point for each win
void markWinner(unordered_map<string, int> &mp, string &winner){
			if(mp.find(winner)==mp.end()){
			  mp.insert({winner,1});	
			}else{
			  mp[winner]+=1;	
			} 
}
//O(n) time and O(k) space - n -number of games and k - is number of winners
string tournamentWinner(vector<vector<string>> competitions,
                        vector<int> results) {
	int i=0;
	unordered_map<string,int> mp;
	int mxTotal=INT_MIN;
	string winner, overAllWinner;
	for(auto a: competitions){
		 if(results[i]){//home team a[0] won if result is 1
			 winner=a[0];
		 }else{//away team a[1] won if result is 0
			 winner=a[1];
		 } 
		 markWinner(mp, winner);
		 if(mxTotal<mp[winner]){
			 mxTotal=mp[winner];
			 overAllWinner = winner; 
		 }
	   i++;	
	}
  return overAllWinner;
}


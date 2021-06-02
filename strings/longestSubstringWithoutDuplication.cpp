#include<vector>
#include<unordered_map>
using namespace std;
//O(n) time and O(min(n,a)) space
string longestSubstringWithoutDuplication(string str) {
  // Write your code here.
	unordered_map<char,int> lastseen;
	vector<int> range={0,1};
	int startidx=0;
	for(int i=0; i<str.length(); i++){
		if(lastseen.find(str[i])!=lastseen.end()){
			startidx=max(startidx,lastseen[str[i]]+1);
		}
		if((i+1-startidx)>(range[1]-range[0])){
			range[0]=startidx;
			range[1]=i+1;
		}
		lastseen[str[i]]=i;
	}
	return str.substr(range[0],range[1]-range[0]);
}

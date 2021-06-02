#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

//O(n*log(w)) time and O(n) space
vector<vector<string>> groupAnagrams(vector<string> words) {
  unordered_map<string,vector<string>> mp;
	for(auto a: words){
		string str=a;
		sort(str.begin(),str.end());
		if(!mp.count(str))
			mp.insert({str,{a}});
		else
			mp[str].push_back(a);
	}
	vector<vector<string>> res;
	for(auto m: mp){
		res.push_back(m.second);
	}
	return res;
	// Write your code here.
}


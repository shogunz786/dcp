/*
 Given two strings check if all chars in one string are part of the other
 string not necessarily in the same order.
  Is  "AlgoExpert is the Best!" made from a subset of chars from
 "Bste!hetsi ogEAxpelrt x ",
 result: true
 */
using namespace std;

bool generateDocument(string characters, string document) {
	unordered_map<char,int> mp;
	for(auto c:characters){
		if(mp.find(c)==mp.end())
			mp[c]=0;
		mp[c]++;
	}
	for(auto c:document){
		if(mp.find(c)==mp.end())
			return false;
		else if(mp[c]<=0)
			return false;
		mp[c]--;
	}
  return true;
}

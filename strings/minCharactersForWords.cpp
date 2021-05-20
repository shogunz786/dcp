/*
 Givn set of words
 "words": ["this", "that", "did", "deed", "them!", "a"]
 find minimum number of characters required to
 make all words.
 results:["!", "a", "d", "d", "e", "e", "h", "i", "m", "s", "t", "t"]
*/
#include <vector>
using namespace std;

//O(nm) time and O(k) space
//n-number of words
//m-size of longest word
vector<char> minimumCharactersForWords(vector<string> words) {
	unordered_map<char,int> gmp;
	for(auto w: words){
		//scan each word and capture unique chars
		//and number of repetitions
	  unordered_map<char,int> mp;
		for(auto c: w){
			if(mp.find(c)==mp.end()){
				mp[c]=0;
			}
			mp[c]++;
		}
		//capture max repetitions in global map
		//and add newly found chars to global map
	  for(auto a:mp){
			if(gmp.find(a.first)==gmp.end()){
				gmp.insert({a.first,a.second});
			}else{
			  gmp[a.first]=max(gmp[a.first],mp[a.first]);	
			}
	  }
	}
	vector<char> res;
	//prepare list of minimum chars
	//required by looking at the global map
	for(auto a:gmp){
	 for(int i=0; i<gmp[a.first]; i++)
	     res.push_back(a.first);	
	}
  return res;
}

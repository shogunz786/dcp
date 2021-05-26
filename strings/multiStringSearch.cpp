/*
Given a bigstring "this is a big string"
find if following small strings are present
["this", "yo", "is", "a", "bigger", "string", "kappa"]

result:
[true, false, true, true, false, true, false]

Solutions:

1. search each small string from each index of big string
2. Use same approach as 1 but match first and last chars simultaneously/
3. Build a trie using big string and check if small strings are present
   in the trie and does not need to check for termination.
4. Build a trie from small string and from each index of big
   string see if any small string occurs by checking for small char termination
   and capture found or not,

*/

#include <vector>
using namespace std;

bool findStr(string smallStr, string bigStr){
	int small_start=0,small_end=0,big_start=0,big_end=0;
	for(int j=0; j<bigStr.length(); j++){
		//check if remaining string is less the small string
		if(bigStr.length()-j<smallStr.length()) continue;
		small_start=0;
		small_end=smallStr.length()-1;
		big_start=j;
		big_end=j+small_end;
		//at each index of big string search both ends
		//for small string
		while(small_start<small_end){
		  if(smallStr[small_start]!=bigStr[big_start] ||
				 smallStr[small_end]!=bigStr[big_end]){
		     break;
		  }
			small_start++;
			small_end--;
			big_start++;
			big_end--;
		}
		//search all of small string without mismatch
		if(small_start>=small_end){
			return true;
		}
	}
	return false;
}
 //time of O(bns) and space O(n)
vector<bool> multiStringSearch1(string bigString, vector<string> smallStrings) {
	vector<bool> res;
	for(auto str: smallStrings){
	    res.push_back(findStr(str,bigString));	
	}
  return res;
}

class TrieNode{
	public:
	string word;
	unordered_map<char, TrieNode*> children;
};

class Trie{
	public:
	TrieNode *root = new TrieNode;
  char endSymbol='*';

	//time O(b^2) and space O(b^2)
	void populateString(string str){
		for(int i=0; i<str.length(); i++){
			TrieNode *node = this->root;
			for(int j=i; j<str.length(); j++){
				char c=str[j];
				if(node->children.find(c)==node->children.end()){
					node->children.insert({c,new TrieNode});
				}
				node=node->children[c];
			}
			node->children.insert({endSymbol,NULL});
			node->word=str.substr(i);
		}
	}
	
	//O(s) time and O(1) space
	bool contains(string str){
			TrieNode *node = this->root;
			for(int j=0; j<str.length(); j++){
				char c=str[j];
				if(node->children.find(c)==node->children.end()){
					return false;
				}
				node=node->children[c];
			}
			return true;
	}
	
};

//time O(b^2+ns) and space O(b^2+n)
vector<bool> multiStringSearchTrieLookinBigString(string bigString, vector<string> smallStrings) {
	Trie t;
	t.populateString(bigString);
	vector<bool> res;
	for(auto str: smallStrings){
	    res.push_back(t.contains(str));	
	}
  return res;
}

void findMatching(string  &bigString, int i,
									unordered_map<string,bool> &mp, Trie &t){
	TrieNode* node = t.root;
	for(int j=i; j<bigString.length(); j++){
		char c=bigString[j];
		if(node->children.find(c)==node->children.end()){
			break;
		}
		node = node->children[c];
		if(node->children.find(t.endSymbol)!=node->children.end()){
			if(mp.find(node->word)==mp.end()){
				mp.insert({node->word,true});
			}
		}
	}
}

//time O(ns to build trie +bs to lookup) and space O(ns) space
vector<bool> multiStringSearch(string bigString, vector<string> smallStrings) {
	Trie t;
	vector<bool> res;
	unordered_map<string,bool> mp;
	//Generate a trie from small strings
	for(auto str: smallStrings){
	    t.populateString(str);
	}
	//from each index of bigstring see if you can find small strings
	for(int i=0; i<bigString.length(); i++){
		findMatching(bigString, i, mp, t);
	}
	//capture all the small strings found
	for(auto str: smallStrings){
		  res.push_back(mp.find(str)!=mp.end());	
	}

  return res;
}


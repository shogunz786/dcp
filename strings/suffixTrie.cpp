#include <unordered_map>
using namespace std;

class TrieNode {
public:
  unordered_map<char, TrieNode *> children;
};

class SuffixTrie {
public:
  TrieNode *root;
  char endSymbol;

  SuffixTrie(string str) {
    this->root = new TrieNode();
    this->endSymbol = '*';
    this->populateSuffixTrieFrom(str);
  }

	//time O(N^2) and space O(N^2)
  void populateSuffixTrieFrom(string str) {
		for(int i=0; i<str.length(); i++){
			//keep adding suffixes of the string to the trie
		  TrieNode *node = this->root;
		  for(int j=i; j<str.length(); j++){
				char c = str[j];
			  if(node->children.find(c)==node->children.end()){
				  node->children.insert({c,new TrieNode()});
			  }
	  		node = node->children[c];
		  }
	  	node->children.insert({this->endSymbol,NULL});
		}
  }

	//time O(N) and space O(1)
  bool contains(string str) {
		TrieNode *node = this->root;
		for(auto c: str){
			if(node->children.find(c)==node->children.end()){
				return false;
			}
			node = node->children[c];
		}
    return node->children.find(this->endSymbol)==node->children.end()?
			     false: true;
  }
};

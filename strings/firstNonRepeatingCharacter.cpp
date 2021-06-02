/*
 Given a string "abcdcaf" find the index of first non-duplicate.
 result: 1
 */
using namespace std;
//O(n) time and O(1) space
int firstNonRepeatingCharacter(string str) {
	vector<int> bitmap(26,0);
	for(auto c:str){
		bitmap[c-'a']++;
	}
	for(int i=0; i<str.length(); i++){
		if(bitmap[str[i]-'a']==1)
			return i;
	}
  return -1;
}

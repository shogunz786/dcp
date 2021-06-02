#include <string>
#include <iostream>
using namespace std;
bool isP(string &str){
	int l=0, r=str.length()-1;
	while(l<r){
		if(str[l]!=str[r])
			return false;
		l++;
		r--;
	}
	return true;
}
//O(n^3) time and O(n) space
string longestPalindromicSubstring1(string str) {
  // Write your code here.
	string longest;
	for(int i=0; i<str.length(); i++){
		for(int j=1; j<=str.length()-i; j++ ){
			string s = str.substr(i,j);
			if(s.length()>longest.length() && isP(s))
				longest = s;
		}
	}
		return longest;
}

vector<int> getP(string &str, int l, int r)
{
	while(l>=0 && r<str.length()){
		if(str[l]!=str[r])
			break;
		l--;
		r++;
	}
	r--;
	l++;
	return {l,r-l+1};
}
//O(n^2) time and O(n) space
string longestPalindromicSubstring(string str) {
	vector<int> res{0,1};
	for(int i=0; i<str.length();i++)
	{
		vector<int> odd = getP(str,i+1,i-1);
		vector<int> even = getP(str,i-1,i);
		if(odd[1]>=res[1])
			res=odd;
		if(even[1]>=res[1])
			res=even;
	}
  return str.substr(res[0],res[1]);
}

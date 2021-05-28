/*
 Given a string of opening and closing brackets.
 Find the longest length of substring with blanaced brackets.
 
 input: "(())("
 output:4
 */
#include <stack>
using namespace std;
bool isBalanced(string str){
	stack<int> st;
	for(int i=0;i<str.length(); i++){
		char a=str[i];
		if(a=='('){
			st.push(i);
		}else if(a==')'){
			if(st.size()==0){
				return false;
			}
			st.pop();
		}
	}
	return st.size()==0;
}

//O(n^3) time and O(n) space
//Check if each substring is balanced and capture max length
int longestBalancedSubstringBF(string str) {
	int longest=INT_MIN;
	//at each index of the string
	for(int l=0;l<str.length();l++){
		//check differnt lengths of the substring
		for(int s=2;s<=str.length()-l; s++){
			if(isBalanced(str.substr(l,s))){
				longest=max(longest,s);
			}
		}
	}
  return longest==INT_MIN?0:longest;
}

//O(n) time and O(n) space
// keep track of begining index, with init set to -1
// keep track of max balanced length and
// replace beginning index as needed
int longestBalancedSubstringIter(string str) {
	int longest=0;
	//init with -1 position
	vector<int> v={-1};
	//at each index of the string
	for(int l=0;l<str.length();l++){
		if(str[l]=='('){
			//push on open bracket
			v.push_back(l);
		}else{
			//pop on closing bracket
			v.pop_back();
			if(v.size()==0){
				//if stack is empty record current position
				v.push_back(l);
			}else{
				//find last index where balanced string ended
				int len=l-v[v.size()-1];
				longest=max(longest,len);
			}
		}
	}
	return longest;
}

//O(n) time and O(1) space
//scan left to right till more closes than opens and record max len
//scan right to left till more opens than closes and record max len
int longestBalancedSubstring(string str) {
	int opening=0;
	int closing=0;
	int longest=0;
	//keep count of openings and closings
	//moving from left to right
	for(auto a: str){
		if(a=='('){
			opening++;
		}else{
			closing++;
		}
		//on match capture longest
		if(opening==closing){
			longest=max(longest,opening*2);
			//when closing>opening means invalid so reset
		}else if(closing>opening){
			opening=0;
			closing=0;
		}
	}
	opening=0;
	closing=0;
	//keep count of openings and closings
	//moving from right to left..match ')' first before '('
	for(int i=str.length()-1; i>=0; i--){
		char a=str[i];
		if(a==')'){
			closing++;
		}else{
			opening++;
		}
		if(opening==closing){
			longest=max(longest,opening*2);
			//reset if openings > closings
		}else if(opening>closing){
			opening=0;
			closing=0;
		}
	}
	return longest;
}
	

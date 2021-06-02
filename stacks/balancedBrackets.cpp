#include <stack>
using namespace std;

//O(n) time and O(n) space
bool balancedBrackets(string str) {
  // Write your code here.
	stack<char> sc;
	string opening("({[");
	string closing(")}]");
	int loc=0;
	for(char a: str)
	{
		if(opening.find(a)!=string::npos){
			sc.push(a);
	  }else if((loc=closing.find(a))!=string::npos){
			if(!sc.empty() && sc.top()==opening[loc])
				sc.pop();
		  else
				return false;
		}
	}
	return sc.empty();
}

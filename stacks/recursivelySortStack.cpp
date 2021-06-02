/*
  sort an array recursively in place using push_back and pop_back
	input [-5, 2, -2, 4, 3, 1]
	result: [-5, -2, 1, 2, 3, 4]
	
	pop all elements from the stack
	then add last element -5
	then add 2
	since -2 can't be added pop 2
	then add -2
	then add 2
	then add 4
	since 3 can't be added as top is 4
	pop 4
	and add 3
	add 4 back
	repeat
*/
#include <vector>
using namespace std;

void insertIntoStack(vector<int> &st, int val){
	//if stack size is zero or top of stack is small
	//push val to top of stack and return
	if(st.size()==0 || st.back()<=val){
		st.push_back(val);
		return;
	}
	//remove top of stack element till top of
	//stack is greater than val
	int top = st.back();
	st.pop_back();
	insertIntoStack(st,val);
	//after removing great values add the top that is used
	//for checking the stack to the back of the stack
	st.push_back(top);
}

//O(n^2) time and O(n) space
vector<int> sortStack(vector<int> &st) {
	//return when st is empty
	if(st.size()==0){
		return st;
	}
	//recursively pop all elements from back
	int top = st.back();
	st.pop_back();
	sortStack(st);
	//insert the last element back into stack
	insertIntoStack(st,top);
  return st;
}


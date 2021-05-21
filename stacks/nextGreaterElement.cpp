/*
 input: [2, 5, -3, -4, 6, 7, 2]
 return array with next greater element at each index
 output: [5, 6, 6, 6, 7, -1, 5]
 */

#include <vector>
#include <stack>
using namespace std;

//O(n) time and O(n) space
vector<int> nextGreaterElement(vector<int> array) {
	//init result array with -1 so highest element will 
	//be replaced with -1
	vector<int> res(array.size(), -1);
	stack<int> st;
	for(int i=0; i<2*array.size(); i++){
		int cIdx = i%array.size();
		//if top of stack is less than cur index
		//set the cur result with top of stack
		//and pop stack to compare with next top of stack
		//keep replacing cur result with stop of stack
		//till current top of stack is less than cur index
		while(st.size()>0 && array[st.top()]<array[cIdx]){
			//replace the result of top of stack with current 
			//array value
			res[st.top()]=array[cIdx];
			//pop the stack
			st.pop();
		}
		//add current index to stack
		st.push(cIdx);
	}
  return res;
}


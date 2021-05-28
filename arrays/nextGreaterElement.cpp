/*
 Given an array of integers
 [2, 5, -3, -4, 6, 7, 2]
 return an array of next greater element by looking thru
 the array as a circular array.
 output:[5, 6, 6, 6, 7, -1, 5]
 */
#include <vector>
using namespace std;

//O(n) time and O(n) space
vector<int> nextGreaterElement(vector<int> array) {
	vector<int> res(array.size(),-1);
	deque<int> dq;
	//trick is to scan thru the array of 2X size
	//and use modulo to rotate indexes and use stack
	//to keep track of smaller elements
	for(int i=0; i<array.size()*2; i++){
		int j=i%array.size();
		while(dq.size() &&
					array[dq.front()]<array[j]){
			 res[dq.front()]=array[j];
			 dq.pop_front();
		 } 
		 dq.push_front(j);
	}
  return res;
}

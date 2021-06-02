#include <stack>
#include <iostream>
using namespace std;

// Feel free to add new properties and methods to the class.
class MinMaxStack {
	stack<int> main, mn, mx;
	public:
	//O(1) and O(1) space
  int peek() {
    // Write your code here.
		if(!main.empty()){
			return main.top();
		}
		return -1;
  }
  //O(1) time and O(1) space
  int pop() {
    // Write your code here.
		int i=-1;
		
		if(!main.empty()){
			i = main.top();
		  if(main.top()==mn.top())
			  mn.pop();
      if(main.top()==mx.top())
			  mx.pop();
		  main.pop();
		}
		return i;
  }

  //O(1) time and O(1) space
  void push(int number) {
    // Write your code here
		if(mn.empty() || mn.top()>=number)
			mn.push(number);
		if(mx.empty() || mx.top()<=number)
			mx.push(number);
		main.push(number);
  }

  //O(1) time and O(1) space
  int getMin() {
    // Write your code here.
		if(!mn.empty()){
			return mn.top();
		}
		return -1;
	}

  //O(1) time and O(1) space
  int getMax() {
    // Write your code here.
		if(!mx.empty()){
			return mx.top();
		}
		return -1;

  }
};

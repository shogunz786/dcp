#include <vector>
using namespace std;

void insertToSortedStack(vector<int> &stack, int value){
  //if last element is less than the value append to stack
  if(stack.size()==0 || stack.back()<=value){
    stack.push_back(value);
    return;
  }
  //remove greater value
  int removedGreaterValue = stack.back();
  stack.pop_back();
  //insert lower value
  insertToSortedStack(stack, value);
  //insert greater value back
  stack.push_back(removedGreaterValue);
}

vector<int> sortStack(vector<int> &stack) {
  if(stack.size()==0){
    return stack;
  }
  //remove reach element from end
  int val = stack.back();
  stack.pop_back();
  //recurse till all elements are removed
  sortStack(stack);
  //start inserting one element at a time
  insertToSortedStack(stack, val);
  return stack;
}

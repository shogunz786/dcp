/*
Given a large array of integers and a window of size w, find the current maximum value in the window as the window slides through the entire array.
input: -4 2 -5 3 6
output: 2,3,6

Solution:
Remove the indices of all elements from the back of the deque, which are smaller than or equal to the current element.
If the element no longer falls in the current window, remove the index of the element from the front.
Push the current element index at the back of the window.
The index of the current maximum element is at the front.
*/

//O(n) time and O(n) space
vector<int> find_max_sliding_window(vector<int>& v, int window_size) {
  vector<int> result;
  
  if (v.size() > 0) {
    //if window size is greater than number of elements
    if ( window_size > v.size()) {
      return result;
    }

    deque<int> window;

    //find out max for first window
    for(int i = 0; i < window_size; ++i) {
      while(!window.empty() && v[i] >= v[window.back()]) {
        window.pop_back();
      }

      window.push_back(i);
    }
    result.push_back(v[window.front()]);
    for (int i = window_size; i < v.size(); ++i) {

      //remove all numbers that are smaller than current number
      //from the tail of list
      while(!window.empty() && v[i] >= v[window.back()]) {
        window.pop_back();
      }

      //remove first number if it doesn't fall in the window anymore
      if(!window.empty() && window.front() <= i - window_size ) {
        window.pop_front();
      }

      window.push_back(i);
      result.push_back(v[window.front()]);
    }
    return result;
  }
  else
    return result;
}
//similiar solution
vector<int> find_max_sliding_window2(vector<int>& v, int window_size) {
 vector<int> result;
 deque<int> temp;
 for(int i=0; i<v.size(); i++){
     while(temp.size() && v[temp.back()]<v[i])
           temp.pop_back();
    temp.push_back(i); 
    if(i>=window_size-1){
         while(temp.size() && temp.front()<=i-window_size)
            temp.pop_front();
       cout<<v[temp.front()]<<endl;
       result.push_back(v[temp.front()]);
    }
 }
 return result;
}

int main(int argc, const char * argv[])
{
  // defining first array
  vector<int> x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};  // 

  // printing first array
  cout << "Array = ";
  for (int i=0; i<x.size(); ++i)
    cout << x[i] << ", ";
  cout << endl;

  auto res1 = find_max_sliding_window(x, 3);
  
  //printing the first result 
  cout << "Max = ";
  for(int i=0; i<res1.size(); ++i)
    cout << res1[i] << ", ";
  cout << endl;
  
  // defining second array
  x = {10, 6, 9, -3, 23, -1, 34, 56, 67, -1, -4, -8, -2, 9, 10, 34, 67};

  //printing the second array
  cout << "Array = ";
  for (int i = 0; i < x.size(); ++i)
    cout << x[i] << ", ";
  cout << endl;

  auto res2 = find_max_sliding_window(x, 3);
  
  //printing the second result
  cout << "Max = ";
  for(int i = 0; i < res2.size(); ++i)
    cout << res2[i] << ", ";
  cout << endl;

  return 0;
}

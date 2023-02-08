#include <vector>
using namespace std;

bool isNotOrdered(int i, int num, vector<int> &array){
  //first element greater than next
  if(i==0){
    return num > array[i+1];
  }
  //last element less than previous element
  if(i == array.size()-1){
    return num < array[i-1];
  }
  //first element greater than next and
  //last element less than previous element
  return num>array[i+1] || num<array[i-1];
}

//O(n) time and O(1) space
vector<int> subarraySort(vector<int> array) {
  //keep track of lowest and highest of
  //out of order range
  int minNumNotInOder = INT_MAX;
  int maxNumNotInOrder = INT_MIN;

  for(int i=0; i<array.size(); i++){
    int num = array[i];
    //when not in order keep track of min and max of the range
    if(isNotOrdered(i, num, array)){
      //keep track of min
      minNumNotInOder = min(minNumNotInOder, num);
      //keep track of max
      maxNumNotInOrder = max(maxNumNotInOrder, num);
    }
  }
  //when all of array is in order
  if(minNumNotInOder==INT_MAX){
    return vector<int>{-1, -1};
  }
  int leftIdx = 0;
  int rightIdx = array.size()-1;
  //travel from 0 to left to find first out of order element
  while(minNumNotInOder >= array[leftIdx]){
    leftIdx++;
  }
  //travel to 0 to right to find first out of order element
  while(maxNumNotInOrder <= array[rightIdx]){
    rightIdx--;
  }
  return vector<int>{leftIdx, rightIdx};
}


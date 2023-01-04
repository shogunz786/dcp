#include <vector>
using namespace std;

void quickSortHelper(vector<int> &array, int startIdx, int endIdx){
  if(startIdx>=endIdx){
    return;
  }
  int pivotIdx=startIdx;
  int leftIdx = startIdx+1;
  int rightIdx = endIdx;
  while(leftIdx<=rightIdx){
    if(array[leftIdx]>array[pivotIdx] && array[rightIdx]<array[pivotIdx]){
      swap(array[leftIdx],array[rightIdx]);
    }else if(array[leftIdx]<=array[pivotIdx]){
      leftIdx++;
    }else if(array[rightIdx]>=array[pivotIdx]){
      rightIdx--;
    }
  }
  swap(array[pivotIdx],array[rightIdx]);
  if(rightIdx-startIdx<endIdx-rightIdx){
    quickSortHelper(array, startIdx, rightIdx-1);
    quickSortHelper(array, rightIdx+1, endIdx);
  }else{
    quickSortHelper(array, rightIdx+1, endIdx);
    quickSortHelper(array, startIdx, rightIdx-1);
  }
}
vector<int> quickSort(vector<int> array) {
  quickSortHelper(array, 0, array.size()-1);
  return array;
}

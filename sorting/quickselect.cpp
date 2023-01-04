#include <vector>
using namespace std;
int quickselectHelper(vector<int> &array, int startIdx, int endIdx, int k)
{
  while(true){
    if(startIdx>endIdx){
      return -1;
    }
    int pivotIdx = startIdx;
    int leftIdx = startIdx+1;
    int rightIdx = endIdx;
    while(leftIdx<=rightIdx){
      if(array[leftIdx]>array[pivotIdx] && array[rightIdx]<array[pivotIdx]){
        swap(array[leftIdx], array[rightIdx]);
      }else if(array[leftIdx]<=array[pivotIdx]){
        leftIdx++;
      }else if(array[rightIdx]>=array[pivotIdx]){
        rightIdx--;
      }
    }
    swap(array[pivotIdx],array[rightIdx]);
    if(rightIdx==k-1){
      return array[rightIdx];
    }else if(rightIdx>k-1){
      endIdx = rightIdx-1;
    }else {
      startIdx = rightIdx+1;
    }
  }
}
int quickselect(vector<int> array, int k) {
  int v = quickselectHelper(array, 0, array.size()-1, k);
  return v;
}

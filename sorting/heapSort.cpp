#include <vector>
using namespace std;
void siftDown(int parentIdx, int endIdx, vector<int>& array){
  int first = 2*parentIdx+1;
  while(first<endIdx){
    int second = 2*parentIdx+2<endIdx?2*parentIdx+2:-1;
    int idxSwap;
    if(second != -1 && array[second]>array[first]){
      idxSwap = second;
    }else{
      idxSwap = first;
    }
    if(array[idxSwap]>array[parentIdx]){
      swap(array[idxSwap], array[parentIdx]);
      parentIdx = idxSwap;
      first = 2*parentIdx+1;
    }else{
      return;
    }
  }
}
void buildMaxHeap(vector<int> &array){
  int firstParentIdx = (array.size()-2)/2;
  for(int i=firstParentIdx; i>=0; i--){
    siftDown(i, array.size(), array);
  }
}
vector<int> heapSort(vector<int> array) {
  buildMaxHeap(array);
  //keep moving max of heap to end and rebuild max heap
  for(int i=array.size()-1; i>0; i--){
    swap(array[0], array[i]);//move max of heap to end
    siftDown(0, i, array);
  }
  return array;
}

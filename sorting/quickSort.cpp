#include <vector>
using namespace std;

int partition(vector<int> &array, int left, int right, int pivot){
  int i=left;
  int j=left;
  while(i<=right){
    if(array[i]>pivot){
      i++;
    }else{
      swap(array[i],array[j]);
      i++;
      j++;//next place to swap
    }
  }
  return j-1;
}

void quickSortHelper(vector<int> &array, int left, int right){
  if(left<right){
    //pick right most as pivor
    int pivot = array[right];
    //find boundary of partitioned arrays
    int pos = partition(array, left, right, pivot);
    //sort right array
    quickSortHelper(array, pos+1, right);
    //sort left array
    quickSortHelper(array, left, pos-1);

  }
}

vector<int> quickSort(vector<int> array) {
  quickSortHelper(array, 0, array.size()-1);
  return array;
}

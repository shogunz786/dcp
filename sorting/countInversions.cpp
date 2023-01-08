#include <vector>
using namespace std;

int mergeInversions(vector<int> &array, int left, int mid, int right){
  int l=left;
  int r=mid;
  vector<int> res;
  int inversions=0;
  while(l<mid && r<right){
    if(array[l]<=array[r]){
      res.push_back(array[l++]);
    }else{
      inversions+=mid-l;
      //number of indices the right elemnt has to be moved
      res.push_back(array[r++]);
    }
  }
  while(l<mid){
      res.push_back(array[l++]);
  }
  while(r<right){
      res.push_back(array[r++]);
  }
  for(int i=0; i<res.size(); i++){
    array[i+left]=res[i];
  }
  return inversions;
}

int countInversionsHelper(vector<int> &array, int left, int right){
  if(right-left<=1){//array of. size <= 1
    return 0;
  }
  int mid=left+(right-left)/2;
  int leftInversions = countInversionsHelper(array, left,mid);
  int rightInversions = countInversionsHelper(array, mid, right);
  int mergeInv=mergeInversions(array, left, mid, right);
  //total inversions are inversion on left partition, right partition
  //and inversions during merge 
  return leftInversions+rightInversions+mergeInv;
}

int countInversions(vector<int> array) {
  return countInversionsHelper(array, 0, array.size());
}

/*
 * Given an array, check if its acending or descending only
 * input: [1,2,3,4,5] or [4,3,2,1] output: true
 *        [1,3,5,0,9] output: false
 */

//time O(N) and space O(1)
bool isMonotonic(vector<int> array) {
  // Write your code here.
  bool decr=true;
  bool incr=true;
  for(int i=1; i<array.size(); i++){
    if(array[i-1]>array[i] && decr){//0>1 incr
      decr = false;
    }else if(array[i-1]<array[i] && incr){//0<1 decr
      incr = false;
    }
    if(decr==false && incr==false){
      return false;
    }
  }
  return decr||incr;
}

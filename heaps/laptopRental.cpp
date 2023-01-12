void siftUp(int currIdx, vector<vector<int>> &v){
  int parentIdx = (currIdx-1)/2;
  while(currIdx>0 && v[currIdx][1]<v[parentIdx][1]){
    swap(v[currIdx],v[parentIdx]);
    currIdx = parentIdx;
    parentIdx = (currIdx-1)/2;
  }
}

void siftDown(int currIdx, int endIdx, vector<vector<int>> &v){
  int childOne = 2*currIdx+1;
  while(childOne<=endIdx){
    int childTwo = 2*currIdx+2<=endIdx?2*currIdx+2:-1;
    int idSwap = childOne;
    if(childTwo!=-1){
      if(v[childTwo][1]<v[childOne][1]){
        idSwap = childTwo;
      }
    }
    if(v[idSwap][1]<v[currIdx][1]){
      swap(v[idSwap], v[currIdx]);
      currIdx=idSwap;
      childOne = 2*currIdx+1;
    }else{
      return;
    }
  }
}
};

int laptopRentals(vector<vector<int>> times) {
  if(times.size()==0){
    return 0;
  }
  sort(times.begin(), times.end(),
    [](vector<int> a, vector<int> b){ return a[0]<b[0]; });
  MinHeap heap({times[0]});
  for(int i=1; i<times.size(); i++){
    //the lowest ending time is less than new interval starting time
    if(heap.peek()[1]<=times[i][0]){
      heap.remove();
    }
    heap.insert(times[i]);
  }
  return heap.size();
}

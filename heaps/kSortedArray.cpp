using namespace std;

bool min_heap_func(int a, int b){ return a<b;}

class Heap{
public:
vector<int> heap;
function<bool(int,int)> compare_func;
Heap(vector<int> v, function<bool(int,int)> fn ){
  compare_func = fn;
  heap = buildHeap(v);
}
int peek(){ return heap[0];}
int remove(){
  int res=peek();
  swap(heap[0],heap[heap.size()-1]);
  heap.pop_back();
  siftDown(0,heap.size()-1, heap);
  return res;
}
void insert(int number){
  heap.push_back(number);
  siftUp(heap.size()-1, heap);
}
int size(){
  return heap.size();
}
vector<int> buildHeap(vector<int> &v){
  int firstParentIdx = (v.size()-2)/2;
  for(int i=firstParentIdx; i>=0; i--){
    siftDown(i, v.size()-1, v);
  }
  return v;
}
void siftUp(int currIdx, vector<int> &v){
  int parentIdx = (currIdx-1)/2;
  while(currIdx>0){
    if(compare_func(v[currIdx],v[parentIdx])){
      swap(v[currIdx],v[parentIdx]);
      currIdx = parentIdx;
      parentIdx=(currIdx-1)/2;
    }else{
      return;
    }
  }
}

void siftDown(int currIdx, int endIdx, vector<int> &v){
  int childOne=2*currIdx+1;
  while(childOne<=endIdx){
    int idSwap = childOne;
    int childTwo = 2*currIdx+2<=endIdx?2*currIdx+2:-1;
    if(childTwo!=-1){
      if(compare_func(v[childTwo],v[childOne])){
         idSwap = childTwo;
      }
    }
    if(compare_func(v[idSwap],v[currIdx])){
         swap(v[idSwap],v[currIdx]);
         currIdx = idSwap;
         childOne = 2*currIdx+1;
    }else{
      return;
    }
  }
}

};

//O(nlogk) time and O(k) space, k is size of heap
vector<int> sortKSortedArray(vector<int> array, int k) {
 
  vector<int> buff(array.begin(), array.begin()+min(k+1, (int)array.size()));
  Heap minHeap(buff, min_heap_func);
  int idx=0;
  
  for(int i=k+1; i<array.size(); i++){
    array[idx++]=minHeap.remove();
    minHeap.insert(array[i]);
  }

  while(minHeap.size()){
     array[idx++]=minHeap.remove();
  }
  return array;
}

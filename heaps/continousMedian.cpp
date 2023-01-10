using namespace std;

bool max_heap_func(int a, int b){ return a>b; }
bool min_heap_func(int a, int b){ return a<b; }

class Heap {
public:
vector<int> heap;
function<bool(int,int)> compare_func;

void print(const char *str){
  cout<<str<<" ";
  for(auto a: heap){
    cout<<a<<",";
  }
  cout<<endl;
}

int size(){
  return heap.size();
}

Heap(vector<int> v, function<bool(int,int)> fn){
  compare_func=fn;
  heap = buildHeap(v);
}

void insert(int num){
  heap.push_back(num);
  siftUp(heap.size()-1, heap);
}

int remove(){
  int res = peek();
  swap(heap[0],heap[heap.size()-1]);
  heap.pop_back();
  siftDown(0, heap.size()-1, heap);
  return res;
}

int peek(){
  return heap[0];
}

//O(n) time and O(1) space
vector<int> buildHeap(vector<int> &v){
  int firstParentIdx = (v.size()-2)/2;
  for(int i=firstParentIdx; i>=0; i--){
    siftDown(i, v.size()-1, v);
  }
  return v;
}
	
//O(logn) time and O(1) space
void siftUp(int currIdx, vector<int> &v){
  int parentIdx = (currIdx-1)/2;
  while(currIdx>0){
    if(compare_func(v[currIdx],v[parentIdx])){
      swap(v[currIdx],v[parentIdx]);
      currIdx = parentIdx;
      parentIdx = (currIdx-1)/2;
    }else{
      return;
    }
  }
}
//O(logn) time and O(1) space
void siftDown(int currIdx, int endIdx, vector<int> &v){
  int childOne = 2*currIdx+1;
  while(childOne<=endIdx){
    int childTwo = 2*currIdx+2<=endIdx?2*currIdx+2:-1;
    int idSwap = childOne;
    if(childTwo!=-1){
      if(compare_func(v[childTwo],v[childOne])){
        idSwap=childTwo;
      }
    }
    if(compare_func(v[idSwap],v[currIdx])){
      swap(v[idSwap],v[currIdx]);
      currIdx=idSwap;
      childOne = 2*currIdx+1;
    }else{
      return;
    }
  }
}
};
class ContinuousMedianHandler {
public:
  double median;
  Heap lowers, greaters;

  ContinuousMedianHandler():lowers({}, max_heap_func), greaters({}, min_heap_func){
    median=0;
  }

  //O(logn) time and O(n) space
  void insert(int number) {
    if(lowers.size()==0 || number<lowers.peek()){
      lowers.insert(number);
    } else{
      greaters.insert(number);
    }
    rebalance();
    updateMedian();
  }

  void rebalance(){
    if(lowers.size()-greaters.size()==2){
      greaters.insert(lowers.remove());
    }else if(greaters.size()-lowers.size()==2){
      lowers.insert(greaters.remove());
    }
  }

  void updateMedian(){
    if(lowers.size()==greaters.size()){
      median = ((double)lowers.peek()+(double)greaters.peek())/2;
    }else if(lowers.size()>greaters.size()){
      median=lowers.peek();
    }else{
      median=greaters.peek();
    }
  }
  double getMedian() { return median; }
};


struct Item{
   int value;
};
class MinHeap{
 private: 
  int size;
  Item *arr;
 public:
  MinHeap(Item *a, int sz):arr(a), size(sz){}
  int left(int i){ return 2*i+1;}
  int right(int i){return 2*i+2;}
  void replaceMin(Item root){arr[0] = root; heapify(0);}
  void heapify(int i);
  Item getMin(){return arr[0];}
  vector<int> getElements();
};

vector<int> MinHeap::getElements()
{
  vector<int> vi;
  for(int i=0; i<size; i++)
  {
     vi.push_back(arr[i].value);
  }
  return vi;
}

MinHeap::MinHeap(Item *a, int sz)
{
  arr = a;
  size = sz;
  int i =(sz-1)/2;
  while(i>=0)
  {
    heapify(i);
    i--;
  }
}

void MinHeap::heapify(int i)
{
  int l = left(i);
  int r = right(i);
  int small = i;
  if(l<size && arr[i].value > arr[l].value)
     small = l;
  if(r<size && arr[i].value > arr[r].value)
     small = r;
  if(small!=i){
     swap(&arr[i],&arr[small]);
     heapify(small);
  }
}


vector<int> topK(vector<int> vi, int k)
{
  if(vi.size() <k)
      return vi;

  Item *it = new Item[k];
  for(int i=0; i<k; i++)
      it[i].value = vi[i];
  MinHeap m(it, k);
  for(int j=k; j<vi.size(); j++)
  {
     Item root = m.getMin();
     if(root.value < vi[j]){
       root.value = vi[j]; 
       m.replaceMin(root);
     }
  }
  return m.getElements();
}

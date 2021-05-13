struct Item{
  int value;
  int r;
  int c;
};

class MinHeap{
  private:
    Item *arr;
    int size;
  public:
    MinHeap(Item a[], int sz);
    int left(int i) {return (2*i+1); }
    int right(int i){return (2*i+2); }
    int replaceMin(Item it){ arr[0]=it; heapify(0);}
    Item getMin(){ return arr[0]; }
    void heapify();
};

MinHeap:MinHeap(Item a[], int sz)
{
  arr = a;
  size = sz;
  int i = (sz-1)/2;
  while(i>=0)
  {
    heapify(i);
    i--;
  }
}

void MinHeap::heapify(int i)
{
  int smallest = i;
  int l = left(i);
  int r = right(i);

  if(l<size && arr[i].value > arr[l].value)
    smallest = l;
  if(r<size && arr[i].value > arr[r].value)
    smallest = r;

  if(smallest !=i)
  {
    swap(&arr[i], &arr[smallest]);
    heapify(smallest);
  }
}

vector<int> mergeArray(vector<vector<int>> vvi)
{
   vector<int> res;
   Item *arr = new Item[vvi.size()];
   
   for(int i=0; i< vvi.size(); i++)
   {
     arr[0].value =  vvi[i][0];
     arr[0].r = i;
     arr[0].c = 0;
   }
   MinHeap heap(arr, vvi.size());
   int r = 0;
   int c = 1;
   while(true)
   {
     Item m = heap.getMin();
     if(m.value == INT_MAX)
        break;
     res.push_back(m.value);
     m.value = INT_MAX;//set max
     if((m.c+1) < vvi[m.r].size())
       m.value = vvi[m.r][m.c+1];//if not end of column replace max
     m.c++;
     heap.replaceMin(m);
   }
   return res;
}
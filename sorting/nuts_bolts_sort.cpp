void matchNB(vector<int> n, vector<int> b, int s, int e)
{
  int p = partition(n,s,e,b[e]);//sort n around b[e]
  partition(n,s,e,n[p]);//sort b around n[p]
  matchNB(n,b,s,p-1);
  matchNB(n,b,p+1,e);
}

int partition(vector<int> arr, int s, int e, int pivot)
{
  int i=s;//marker to know the most recent lowest value
  for(int j=s; j<e; j++)
  {
    if(arr[j]<pivot)
    {
      swap(&arr[i],&arr[j]);
      i++;
    }else if(arr[j]==pivot)
    {
      swap(&arr[j],&arr[e]);//move match to end
      j--; //reexamine this element after swap
    }
  }
  swap(&arr[i],&arr[e]);//move match to first element that was more than pivot
  return i;
}

  

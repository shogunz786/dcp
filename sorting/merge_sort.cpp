vector<int> merge(vector<int> a, vector<int> b){
  if(a.size()==0)
       return b;
  if(b.size() == 0)
       return a;
  vector<int> res;
  int ai=0, bi=0;
  int sz = a.size()+b.size();
  for(int i=0; i<sz; i++)
  {
    if(ai<a.size() && bi<b.size())
    {
       if(a[ai]<b[bi]){
         res.push_back(a[ai]);
         ai++;
       }else{
         res.push_back(a[bi]);
         bi++;
       }
    }else if(ai>=a.size() && bi<b.size())
    {
       res.push_back(a[bi]);
       bi++;
    }else if(ai<a.size() && bi>=b.size())
    {
       res.push_back(a[ai]);
       ai++;
    }
  }
  return res;
}

void merge_change_input_array(vector<int> &arr, int l, int m, int r)
{
  vector<int> a,b;
  for(int i=l;i<m+1;i++)
       a.push_back(arr[i]);
  for(int j=m+1;j<r+1;j++)
       b.push_back(arr[j]);
  int ai=0, bi=0, k=l;
  while(ai<a.size() && bi<b.size())
  {
     if(a[ai]>b[bi])
     {
       arr[k++]=a[ai++];
     }else{
       arr[k++]=b[bi++];
     }
  }
  while(ai<a.size())
  {
     arr[k++]=a[ai++];
  }
  while(bi<b.size())
  {
     arr[k++]=b[bi++];
  }
}

void msort_change_input_array(vector<int>& arr, int l, int r)
{
   if(l<r){
     int m= l+(r-l)/2;
     msort_change_input_array(arr, l, m);
     msort_change_input_array(arr, m+1, r);
     merge_change_input_array(arr,l,m,r);
   }   
}

void merge_const_extra_space(vector<int> &res, int l, int m, int r)
{
  int left =l, right=m+1;
  //middle is less than next so range already sorted
  if(res[m]<res[m+1])
    return;
  while(left<=m && right<=r ){
    //move left if right is greater
    if(res[left]<res[right]){
         left++;
    }else{
      //get the greater item into temp
      int temp = res[right]; 
      //move from left to right by one to right
      for(int j=right-1; j>=left; j--)//move the array right
      {
        res[j+1] = res[j]; 
      }
      res[left]=temp;
      //inc as everything moved by one
      left++;
      right++;
      m++;
    }
  }
}

void msort_const_extra_space(vector<int>& arr, int l, int r, vector<int> &res)
{
   if(l==r){
      res[l]=arr[l];//load up the constant space
      return;
   }
   if(l<r){
     int m= l+(r-l)/2;
     msort_const_extra_space(arr, l, m, res);
     msort_const_extra_space(arr, m+1, r, res);
     merge_const_extra_space(res,l,m,r);
   }   
}

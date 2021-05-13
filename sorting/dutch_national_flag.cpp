
void sortDutchFlag(vector<int> &vi)
{
  int l=0;
  int r=vi.size()-1;
  
  for(int i=0; i<=r; i++)
  {
     if(vi[i]==0){
        //l to mark the next pos for zero swap
        swap(vi[l++],vi[i]);
     }else if(vi[i]==2){
        //r to mark the next pos for 2 swap
        //check i again after swap.
        //increment i to compensate the for loop increment
        swap(vi[r--],vi[i--]);     
     }
  }
}
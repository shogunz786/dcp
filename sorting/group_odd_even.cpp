void groupNumbers(vector<int> vi)
{
  int i = -1;
  int e = vi.size();
  for(int j=0; j<e; j++)
  {
     if(vi[i+1]%2==0)
     { 
       i++;
       swap(&vi[i],vi[j]);
     }else{
       swap(&vi[i+1],vi[e-1]);
       j--;
       e--;//last element already odd,next time swap in prev element
     }
  }
}
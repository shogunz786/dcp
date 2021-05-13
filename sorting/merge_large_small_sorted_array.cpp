void fillfromEnd(vector<int> &large)
{
  int j=large.size()-1;
  for(int i=large.size()-1; i>=0; i--)
  {
    if(large[i]!=-1)
    {
       swap(large[i],large[j]);
       j--;
    }
  }
}

void merge(vector<int> &large, vector<int> &small)
{
  int l = small.size();
  int s = 0;
  int l_b = 0;
  //move all the -1's to front and start merge
  fillefromEnd(large);
  while(l_b<large.size())
  {
    if((s<small.size()&& l<large.size() && large[l]<=small[s]) ||
       s>=small.size())
    //update large if less than small or small exhausted
    {
      large[l_b]=large[l];
      l++;
      l_b++;
    }else
    //else move small to large
    {
      large[l_b]=small[s];
      s++;
      l_b++;
    }
  }
}


int partition(vector<int>& vi, int l, int r)
{
   int pivot = vi[r];
   int i = l; 
   for(int j=l; j<r; j++)
   {
      if(vi[j]<=pivot)
      {
	  swap(vi[i],vi[j]);
	  i++;
      }
   }
   swap(vi[i], vi[r]);
   return i;
}


void findNearest(vector<int> &vi, int k)
{
	int l = 0;
	int r = vi.size()-1;
	int pivot = 0;
	while(l<r)
	{
		pivot = partition(vi, l, r);
		if(pivot==k){
			break;
		}else(pivot<k){
			l=pivot+1;
		}else
			r=pivot-1;
	}
	for(int i=0; i<k; i++)
		cout<<vi[k]<<",";
	cout<<endl;
}

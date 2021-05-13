void pascal_traingle(int n)
{
	vector<int> prev(n,0), cur(n,0);
	long sum =0;
	for(int i=1; i<=n; i++)//different sizes
	{
		for(int j=0; j<i; j++)
		{
			if(j==0 || j+1==i) cur[j]==1;//edges set to 1
			else cur[j]=prev[j]+prev[j-1];//cur+prev 
			sum+=(i*cur[j])+(j+1)
		}
		for(int j=0; j<i; j++)
		{
			cout<<cur[j]<<" ";
			prev[j]=cur[j];
		}
		cout<<endl;
	}
}

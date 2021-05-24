/*
Pascal’s triangle is a triangular array of the binomial coefficients. Write a function that takes an integer value
n as input and prints first n lines of the Pascal’s triangle. Following are the first 6 rows of Pascal’s Triangle.
 

1  
1 1 
1 2 1 
1 3 3 1 
1 4 6 4 1 
1 5 10 10 5 1 
*/
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

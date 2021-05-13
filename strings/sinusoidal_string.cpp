void sinusoidal(string str, int n)
{
	char out[n][str.length()]={};
	int r=n-1, c=0, up=1;
	for(int i=0; i<str.length();)
	{
		out[r][i]=str[i];
		if(r==0)//on top go down
			up=0;
		if(r+1==n)//on bottom go up
			up=1;
		if(up)
			r--;//go up
		else
			r++;//go down
	}
	for(int j=0;j<n;j++)
	{
		for(int k=0; k<str.length(); k++)
			cout<<out[j][k]<<" ";
		cout<<endl;
	}
}

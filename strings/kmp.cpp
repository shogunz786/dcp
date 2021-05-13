vector<int> computeTemporaryArray(string pattern)
{
	int j = 0;
	int i=1;
	vector<int> arr(pattern.length(), 0);
	while(i<pattern.length())
	{
		if(pattern[i]==pattern[j])
		{
			j++;
			arr[i++]=j;
		}else{
			if(j)
				j=arr[j-1];
			else
				arr[i++]=0;
		}
	}
}

void serachKMP(string text, string pattern, vector<int> kmparr)
{
	vector<int> kmparr = computeTemporaryArray(pattern);
	int pi = 0, ti=0;
	while(ti<text.length() && pi<pattern.length())
	{
		if(text[ti]==pattern[pi]){
			ti++;
			pi++;
		}
		if(pi == pattern.length()){
			cout<<"Found match at"<<(ti-pi)<<endl;
			pi=kmparr[pi-1]
		}
		if(ti<text.length() && text[ti]!=pattern[pi]){
			if(pi) pi=kmparr[pi-1];
			else
				ti++;
		}
	}
}

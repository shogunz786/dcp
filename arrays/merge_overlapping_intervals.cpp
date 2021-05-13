bool sortfunc(vector<int> a, vector<int> b)
{
	if(a.size() !=2 || b.size()!=2) return false;
	return a[0]<b[0];
}
vector<vector<int>> mergeIntervals(vector<vector<int>> arr)
{
	sort(arr.begin(), arr.end(), sortfunc);
	vector<vector<int>> res;
	res.push_back(arr[0]);
	for(int i=1; i<arr.size(); i++)
	{//prev a,b cur c,d  c>b so no overlap
		if(arr[i][0]>res.back()[1]) res.push_back(arr[i]);
		else  res.back()[1]=max(res.back()[1],arr[i][1]);//get max end
	}
	return res;
}
//cardinal numbers for begin and end
vector<vector<int>> mergeIntervals(vector<vector<int>> arr)
{
	vector<vector<int>> res;
	if(arr.size()==0)
		return res;
	map<int,vector<int>> mp;//a index can be begin and start for some ranges
	int min=INT_MAX, max=INT_MIN;
	for(int i=0; i<res.size(); i++)
	{
		min=min(res[i][0],min);
		max=max(res[i][1],max);
		if(mp.find(res[i][0])==mp.end())
			mp.insert(make_pair(res[i][0],vector<int>(1,0)));
		else
			mp[res[i][0]].push_back(0);

		if(mp.find(res[i][1])==mp.end())
			mp.insert(make_pair(res[i][1],vector<int>(1,1)));
		else
			mp[res[i][1]].push_back(1);
	}
	stack<int> si;
	for(int j=min; j<=max; j++)
	{
		if(mp.find(j)!=mp.end())
		{
			vector<int> vi = mp[j];
			for(int k=0; k<vi.size(); k++)
			{
				if(vi[k]==0)//push if begin of range
					si.push(j);
				else{//pop if end of a range
					int tp = si.top();
					si.pop();
					if(si.empty())//if no outstanding begins declare range
						cout<<tp<<","<<j<<endl;
				}
			}
		}
	}
}

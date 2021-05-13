void addtoVStr(vector<string> &vstr, vector<int> arr, pair<int,int> p)
{
	stringstream ostr;
	for(int i=p.first; i<=p.second; i++)
		ostr<<arr[i]<<" ";
	vstr.push_back(ostr.str());
}
vector<string> sumzero(vector<int> arr)
{
	vector<string> result;
	unordered_map<int,vector<int>> sum_mp;
	int sum = 0;
	for(int i=0; i<arr.size(); i++)
	{
		sum += arr[i];
		if(!sum)
			addToVstr(result, arr, {0,i});

		if(sum_mp.find(sum)!=sum_mp.end())//a range already reached this sum so in between the sum reached zero
		{//ignore the first element if its zero and not the only element 
            vector<int> v = sum_mp[sum];
			for(auto a: v)
				addToVstr(result, arr, {a,i});
		}
		sum_map[sum].push_back(i);
	}
	return result;
}

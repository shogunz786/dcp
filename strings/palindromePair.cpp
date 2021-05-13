vector<vector<int>> gtePalindromePairs(vector<string> vs)
{
	vector<vector<int>> vvi;
	unordered_map<string, int> mp;
	for(int i=0; i<vs.size(); i++)
		mp.insert(make_pair(vs[i],i));

	for(int j=0; j<vs.size(); j++)
	{//abcdefg
		for(int k=0; k<vs[j].length(); k++)
		{//check defg for palindrome
                   if(isPalindrome(vs[j],k+1,vs[j].length()-1)){
			   string prefix = vs[j].substr(0,k+1);//check if cba has a match
			   reverse(prefix.begin(),prefix.end());
			   if(mp.find(prefix)!=mp.end() && j!=mp[prefix])
			   {
                                 vector<int> v;
				 v.push_back(j);
				 v.push_back(mp[prefix]);
				 vvi.push_back(v);
			   }
		   }
                   if(isPalindrome(vs[j],0,k)){
			   string suffix = vs[j].substr(k+1);
			   reverse(suffix.begin(),suffix.end());
			   if(mp.find(suffix)!=mp.end() && j!=mp[suffix])
			   {
                                 vector<int> v;
				 v.push_back(j);
				 v.push_back(mp[suffix]);
				 vvi.push_back(v);
			   }
		   }
		}
	}
	return vvi;
}

bool isPalindrome(string str, int l, int r)
{
	if(l>r)
		return false;
	while(l<r){
		if(str[l++]!=str[r--])
			return false;
	}
	return true;
}

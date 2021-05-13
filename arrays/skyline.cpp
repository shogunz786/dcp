vector<vector<int>> find_skyline(vector<vector<int>> building)
{
	vector<vector<int>> res;
	vector<pair<int,int>> edges;
	int prev=0, cur=0;
	for(int i=0; i<building.size(); i++)
	{
		edges.push_back(make_pair(building[i][0],-building[i][1]));//use -ve number for begging height
		edges.push_back(make_pair(building[i][2],building[i][1]));
	}
	sort(edges.begin,edges.end());
	multiset<int> ms;//stores in order and allows duplicates
        ms.insert(0);
	for(int i=0; i<edges.size(); i++)
	{
		if(edges[i].second<0)
			ms.insert(-edges[i].second);//insert beginning height
		else
			ms.erase(edges[i].second);//can delete all the matching heights
                cur = *(ms.rbegin());//when no tallest height return 0 added at first
		                     //return last element with rbegin
		if(cur != prev)//cur height is not same as last processed height
		{
			prev=cur;
			vector<int> temp;
			temp.push_back(edges[i].first);
			temp.push_back(cur);
			res.push_back(temp);
		}
	}
	return res;
}

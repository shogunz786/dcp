bool adjacent_word(string src, string dest)
{
	int count=0, i=0;;
	if(src.length()!=dest.length()) return false;
	while(count<=1 && i<src.length())
	{
	       	if(src[i]!=dest[i])
			count++;
		i++;
	}
	return count==1?true:false;
}
clas Item{
	public:
		string cur;
		int distance;
		Item(string str,int d){cur=str; distance=d;}
};
vector<string> string_transformation(vector<string> words, string start, string stop)
{
	unordered_map<string, int> wmap;
	vector<bool> visited(words.size(),false);
	vector<int> parent(words.size(),-1);
	queue<Item> q;
	int found=0;

	for(int i=0; i<words.size();i++)
		wmap.insert(make_pair(words[i],i));

	Item it(start,wmap[start]);
	q.push(it);
	visited[wmap[start]]=true;
	while(!q.empty())
	{
		it = q.front();
		q.pop();
		if(it.cur.compare(stop)==0){//the first transformation that reached is the shortest
			found=1;
			break;
		}
#ifdef WORD_LIST_SMALL
		for(int i=0;i<words.size();i++)
		{
			if(visited[wmap[words[i]]]==false && adjacent_word(it.cur,words[i]))
			{//add only not visited and adjacent words
				Item t(words[i],it.distance+1);
				visited[i]=true;
				q.push(t);
				parent[i]=wmap[it.cur];
			}
		}
	}
#else //instead of seraching all the array, change one char and check for match
	string str = it.cur;
	for(int i=0; i<it.cur.length() && !found;i++)
	{
		for(char ch='a'; ch<='z' && !found; ch++)
		{
			char orig=str[i];
			if(ch==orig)
				continue;
			str[i]=ch;//change by one char and search
			if(wmap.find(str)!=wmap.end() &&
			   visited[wmap[str]]==false)
			{
				visited[wmap[str]]=true;
				parent[wmap[str]]=wmap[t.cur];
				if(str.compare(stop)==0){
					found=1;
					break;
				}
				Item t(str,it.distance+1);
				q.push(t);

			}
			str[i]=orig;//restore original string
		}
	}
#endif

	vector<string> path;
	if(found){
		path.push_back(stop);
		while(start.compare(stop)!=0)
		{
			stop = words[parent[wmap[stop]]];
			path.push_back(stop);
		}
		reverse(path.begin(), path.end());
	}
	return path;
}

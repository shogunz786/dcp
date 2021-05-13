struct Graph{
	int V;
	list<int> *adj;
	Graph(int n){V=n; adj=new list<int>[n];}
	void topoSort();
	void topoSortUtil(int v, stack<int> &s, vector<bool> &visited);
};
void Graph::topoSortUtil(int v, stack<int> &s, vector<bool> &visited){
	visited[v]=true;
	list<int>::iterator iter=adj[v].begin();
	for(; iter!=adj[v].end();iter++)
	{
		if(visited[*iter]==false){
			topoSortUtil(*iter, s, visited);
		}
	}
	s.push(v);//visit all neighbors and their neighbors and add to stack
}
void Graph::topoSort()
{
	vector<bool> visited(V, false);
	stack<int> s;
	for(int i=0; i<V; i++)
	{//recursively run topo sort on all edges
		if(visited[i]==false)
			topoSortUtil(i, s, visited);
	}
	//print contents of the stack
	while(!s.empty())
	{
		cour<<(char)(('a'+s.top());
		s.pop();
	}
}
void find_order(vector<string> words, int no_of_chars)
{
	Graph g(no_of_chars);//strongly connected graph
	for(int i=0; i<words.size()-1; i++)
	{//take smaller of the words and compare
		int minsz = min(words[i],words[i+1]);
		for(int j=0; j<minsz; j++)
		{
			if(words[i][j]!=words[i+1][j])
			{//add the first mismatch as edge
				g.adj[words[i][j]-'a'].push_back(words[i+1][j]-'a');
				break;
			}
		}
	}
	g.topoSort();//run topo sort on the created graph
}

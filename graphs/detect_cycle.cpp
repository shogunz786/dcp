class Graph{
	public:
		int V;
		list<int> *neighbors;
		void addEdge(int v, int u){
			neighbors[v].push_back(u);
		}
		Graph(int noofv){ 
			V=noofv;
			neighbors = new list<int>[V];
	       	}
		bool isCycleDFS();
		bool isCycleDFSUtil(int u);
};

bool isCycleDFSUtil(int u, vector<bool> &visited, vector<bool> &recStack)
{
	if(visited[u]==false)//ignore already visited
	{
		visited[u]=true;
		recStack[u]=true;//mark as visited in stack
		list<int>::iterator iter=neighbors[u].begin();
		for(;iter!=neighbors[u].end(); iter++)
		{//if not visited check if there is cycle
			if(visited[*iter]==false && isCycleDFSUtil(*iter, visited, recStack))
				return true;
			else if(recStack[*iter])
				return true;
		}
	}
	recStack[u]=false;
	return false;
}

bool Graph::isCycleDFS()
{
	vector<bool> visited(V, false);
	vector<bool> recStack(V, false);
	for(int i=0; i<V; i++)//if cycle return else continue to process
		if(isCycleDFSUtil(i, &visited, recStack))
			return true;
}

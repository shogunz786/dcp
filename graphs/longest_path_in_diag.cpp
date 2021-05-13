class Graph{
	public:
	int V;
	list<pair<int,int>> *adj;
	Graph(int n){V=n; adj=new list<pair<int,int>>[n];}
	void addEdge(int u, int v, int w){
		adj[u].insert(make_pair(v,w));
	}
	void topoSort(stack<int> &st);
	void topoSortUtil(int u, stack<int> &st, vector<bool> &visited);
};
void Graph::topoSortUtil(int u, stack<int> &st, vector<bool> &visited)
{
	visited[u]=true;
	list<pair<int,int>>::iterator = adj[u].begin();
	for(;iter!=adj[u].end(); iter++)
	{
		if(visited[(*iter).first]==false)
			topoSortUtil((*iter).first, st, visited);
	}
	st.push(u);
}
void Graph::topoSort(stack<int> &st)
{
	vector<bool> visited(V, false);
	for(int i=0; i<V; i++)
	{
		if(visited[i]==false)
			topoSortUtil(i, st, visited);
	}
}
vector<int> find_longest_path(int dag_nodes, vector<int> dag_from, vector<int> dag_to, vector<int> dag_weight, int from_node, int to_node)
{
	vector<int> distances(dag_nodes, INT_MIN);
	vector<int> parent(dag_nodes, 0);
	distances[from_node]=0;//starting from from_node
	stack<int> st;
	//build graph
	Graph g(dag_nodes);
	for(int i=0; i<dag_nodes; i++)
		g.addEdge(dag_from[i], dag_to[i], dag_weight[i]);
        //run topo sort
	g.topoSort(st);
	//process topological order
	while(!st.empty())
	{
		int from = st.top();
		st.pop();
		if(from==to_node) break; //reached the order till we care about
		if(distances[from]!=INT_MIN){//start with from_node and process neighbors
			list<pair<int,int>>::iterator iter = adj[from].begin();
			for(;iter!=adj[from].end();iter++)
			{//if current neighbor distance from u is less update distance
				int to = (*iter).first;
				int to_edge_wt = (*iter).second;
				if(distances[to]<=(distances[from]+to_edge_wt))
				{
				   distances[to]=distances[from]+to_edge_wt;
				   parent[to]=from;
				}
			}
		}
	}

	//build path
	vector<int> path;
	path.push_back(to_node);
	while(to_node != from_node)
	{
		to_node = parent[to_node];
		parent.push_back(to_node);
	}
	reverse(path.begin(), path.end());
	return path;
}

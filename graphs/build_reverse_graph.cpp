class Graph{
  public:
    int V;
    list<int> *adj;
    Graph(int n):V(n){adj=new list<int>[n];}
    void addEdge(int u, int v){ adj[u].push_back(v); }
};

Graph *getReverseClone(Graph *src)
{
	Graph *g = new Graph(src->V);
	for(int i=0;i<src->V; i++)
	{
		list<int>::iterator = src->adj[i].begin();//iterate neighbors
		for(;iter!=src->adj[i].end(); iter++)
		{
			g->adj[*iter].push_back(i);
		}
	}
	return dest;
}

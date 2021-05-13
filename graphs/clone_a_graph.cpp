class Graph{
  public:
    int V;
    list<Graph*> adj;
    Graph(int n):V(n), adj(n,NULL){}
    void addEdge(Graph *v){ adj.push_back(v); }
    Graph *Graph::getClone();
};

Graph *getClone(Graph *src)
{
	map<Graph*, Graph*> oldnewmp;//map of src and dest nodes
	queue<Graph*> q;
	if(!src) return NULL;
	q.push(src);//load first node
	Graph *dest = new Graph(src->V);
	oldnewmp.insert(make_pair(src,dest));
	while(!q.empty())
	{
		src = q.front();//get top
		q.pop_front();
		list<Graph*>::iterator = src->adj.begin();//iterate neighbors
		for(;iter!=src->adj.end(); iter++)
		{//make a map of new nodes and add to queue
			if(oldnewmp.find(*iter)==oldnewmp.end()){
				oldnewmp.insert(make_pair(*iter,new Graph((*iter)->V)));
				q.push(*iter);
			}//make a neighbor copy
			oldnewmp[src].addEdge(oldnewmp[*iter]);
		}
	}
	return dest;
}

void bfs(Graph *g)//to verify
{
	queue<Graph*> q;
	map<Graph*,bool> visited;
	q.push(g);
	visited.insert(make_pair(g,true));
	while(!q.empty())
	{
		g = q.front();
		cout<<g->V<<" ";
		cout<<" neighbors :";
		q.pop();
		list<Graph*>::iterator iter=g->adj.begin();
		for(;iter!=g->adj.end(); iter++)
		{
			if(visited.find(*iter)==visited.end())
			{
				visited.insert(make_pair(*iter,true));
				q.push(*iter);
			}
			cout<<(*iter)->V<<" ";
		}
		cout<<endl;
	}
}

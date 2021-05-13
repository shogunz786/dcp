//assumptions moves[i]==-1 only goes one cell at a time
//moves[i]>i ladder
//moves[i]<i snake 
//n number of cells
struct Turn{
	int cell;
	int rolls;
        Turn(){cell=0; rolls=0; }
};
int minThrows(int n,vector<int> moves) 
{
	queue<Turn> q;
	Turn t;
	int c;
	vector<bool> visited(n, false);
	q.push(t);//add first cell
	visited[t.cell]=true;//mark visited
	while(!q.empty())
	{
		t = q.front();
		c = t.cell;
		if(c == n-1) break;//check if reached end
		q.pop();
		for(int i=1+c; i<=(6+c) && i<n; i++)
		{
			if(visited[i]==false){
				Turn u;
				visited[i]=true;
				//set current cell or next 
				//cell based on snake or ladder
				u.cell=(moves[i]==-1)?i:moves[i];
				u.rolls=t.rolls+1;
				q.push(u);
			}
		}
	}
	return t.rolls;
}

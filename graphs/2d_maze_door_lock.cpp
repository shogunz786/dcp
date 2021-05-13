#ifdef BRUTE_FORCE
void solve(int x,int y, vector<string> grid, map< pair<int,int>, int > visited, vector<
vector<int> > path,vector< vector<int> >& short_path, map<char, int> keys )
{
	if(x<0 || y<0 || x>=grid.size() || y>=grid[0].size())
		return;
	//cout<<"1111"<<"("<<x<<","<<y<<") = ["<<grid[x][y]<<"]"<<endl;
	if(grid[x][y] != '@' && grid[x][y] != '+' && grid[x][y] != '#' &&
			!(grid[x][y] >= 'a' && grid[x][y]<='z') && grid[x][y] != '.' &&
			!(grid[x][y] >= 'A' && grid[x][y]<='Z') )
		return;
	if(visited[make_pair(x,y)]!=0)
		return;
	if((path.size()+2 >= short_path.size() &&
				short_path.size()!=0)||path.size() > (2*grid.size()*grid[0].size())){
		//cout<<"4444444"<<endl;
		return;
	}
	//cout<<"22222"<<endl;
	if(grid[x][y] == '+'){
		//cout<<"found path"<<endl;
		vector<int> vi;
		vi.push_back(x);
		vi.push_back(y);
		short_path = path;
		short_path.push_back(vi);
		return;
	}
	if(grid[x][y] >= 'a' && grid[x][y]<='z'){
		//cout<<"5555555"<<endl;
		keys[grid[x][y]]=1;
		//reinit visited
		for(int i=0;i<grid.size();i++){
			for(int j=0;j<grid[0].size();j++)
			{
				if(visited[make_pair(i,j)]==1)
					visited[make_pair(i,j)]=0;
			}
		}
	}else if(grid[x][y] >= 'A' && grid[x][y]<='Z'){
		//cout<<"66666666"<<endl;
		if(keys[tolower(grid[x][y])]==0)
			return;
	}
	//cout<<"777777"<<endl;
	if(grid[x][y] == '@'){
		visited[make_pair(x,y)]=2;
	}else
		visited[make_pair(x,y)]=1;//mark visited
	vector<int> vi;
	vi.push_back(x);
	vi.push_back(y);


	vi.push_back(y);
	path.push_back(vi);//add to path
	solve(x+1,y,grid,visited,path,short_path,keys);
	solve(x,y+1,grid,visited,path,short_path,keys);
	solve(x-1,y,grid,visited,path,short_path,keys);
	solve(x,y-1,grid,visited,path,short_path,keys);
	path.pop_back();
	visited[make_pair(x,y)]=0;
}
/*
* Complete the function below.
*/
vector < vector<int> > find_shortest_path(vector <string> grid) {
	int x,y;
	map< pair<int,int>, int> visited;
	vector< vector<int> > vvi;
	vector< vector<int> > short_vvi;
	map<char,int> keys;
	if(grid.size()==0 || grid[0].size()==0)
	{
		vector<int> vi;
		vi.push_back(-1);
		vvi.push_back(vi);
		return vvi;
	}
	for(int i=0;i<grid.size();i++)
	{
		for(int j=0;j<grid[0].size();j++)
		{
			if(grid[i][j]=='@')
			{
				x = i;
				y = j;
			}
			if(grid[i][j] >= 'a' && grid[i][j]<='z')
				keys.insert(make_pair(grid[i][j],0));
			if(grid[i][j]=='#')
				visited.insert(make_pair(make_pair(i,j),2));
			else
				visited.insert(make_pair(make_pair(i,j),0));
		}
	}
	solve(x,y,grid,visited,vvi,short_vvi,keys);
	return short_vvi;
}
#endif
//solved with DP
const int MAX_ROWS = 100, MAX_COLS = 100, MAX_KEYS = 10, MAX_MASK = (1 << MAX_KEYS), INF = MAX_ROWS * MAX_COLS * (MAX_KEYS + 1);
const int add_r[4] = {-1, 0, 1, 0};
const int add_c[4] = {0, -1, 0, 1};
bool is_start(char ch)
{
	return ch == '@';
}
bool is_stop(char ch)
{
	return ch == '+';
}
bool is_water(char ch)
{
	return ch == '#';
}
bool is_land(char ch)
{
	return ch == '.';
}
bool is_key(char ch)
{
	return ('a' <= ch && ch < ('a' + MAX_KEYS));
}
bool is_door(char ch)
{
	return ('A' <= ch && ch < ('A' + MAX_KEYS));
}
bool can_open_door(char door, int ring_of_keys)
{
	return (ring_of_keys >> (door - 'A')) & 1;
}
//capture where did he come from, with what keys and what distance from start
void add_neighbour_to_queue(int to_r, int to_c, int to_ring_of_keys,
pair<pair<int, int>, int> from, vector<vector<vector<pair<pair<int, int>, int>>>>
&parent,
vector<vector<vector<int>>> &dp, vector<vector<vector<bool>>> &visited,
queue<pair<pair<int, int>, int>> &q)
{
	parent[to_r][to_c][to_ring_of_keys] = from;//from came with to ring of keys
	dp[to_r][to_c][to_ring_of_keys] = dp[from.first.first][from.first.second][from.second] +
		1;//increment distance
	visited[to_r][to_c][to_ring_of_keys] = true;//mark visited with so many keys
	q.push({{to_r, to_c}, to_ring_of_keys});
}
// better to pass vectors by reference
void bfs(vector<string> &grid, pair<int, int> &start, vector<vector<vector<int>>> &dp,
vector<vector<vector<pair<pair<int, int>, int>>>> &parent,
vector<vector<vector<bool>>> &visited)
{
	int rows = grid.size();
	int cols = grid[0].length();
	queue<pair<pair<int, int>, int>> q;
	// Starting point with no keys has 0 distance (itself).
	q.push({start, 0});
	dp[start.first][start.second][0] = 0;
	visited[start.first][start.second][0] = true;
	while(q.empty() == false)
	{
		pair<pair<int, int>, int> from = q.front();
		q.pop();
		if (is_stop(grid[from.first.first][from.first.second]))
		{
			continue;
		}
		// Visit all four neighbours and update.
		for (int i = 0; i < 4; i++)
		{
			int to_r = from.first.first + add_r[i];
			int to_c = from.first.second + add_c[i];
			if (to_r < 0 || to_r >= rows || to_c < 0 || to_c >= cols)
			{
				continue;
			}
			if (is_water(grid[to_r][to_c]))
			{
				continue;
			}
			else if (is_land(grid[to_r][to_c]) || is_start(grid[to_r][to_c]) ||
					is_stop(grid[to_r][to_c]))
			{
				if (visited[to_r][to_c][from.second] == false)//not visited without key
				{
					{
						add_neighbour_to_queue(to_r, to_c, from.second, from, parent, dp, visited, q);
					}
				}
				else if (is_key(grid[to_r][to_c]))
				{
					int new_ring_of_keys = from.second | (1 << (grid[to_r][to_c] - 'a'));//add to existing keys
					if (visited[to_r][to_c][new_ring_of_keys] == false)//did not vist with those keys
					{
						add_neighbour_to_queue(to_r, to_c, new_ring_of_keys, from, parent, dp,
								visited, q);
					}
				}
				else if (is_door(grid[to_r][to_c]))
				{
					// Can enter only if we have key for that door.
					if (can_open_door(grid[to_r][to_c], from.second))//can open door with one of the keys
					{
						if (visited[to_r][to_c][from.second] == false)
						{
							add_neighbour_to_queue(to_r, to_c, from.second, from, parent, dp,
									visited, q);
						}
					}
				}
			}
		}
	}
}
vector<vector<int>> build_path(vector<vector<vector<int>>> &dp,
	       	vector<vector<vector<pair<pair<int, int>, int>>>> &parent,
	       	int ring_of_keys,
	       	pair<int, int> &start,
	       	pair<int, int> &stop)
{
	vector<vector<int>> ans;
	ans.push_back(vector<int>(0));
	ans[0].push_back(stop.first);
	ans[0].push_back(stop.second);
	/*
	   stop only when ring_of_keys is 0. Both conditions are necessary. Consider input 1 5
	   "#a@A+#",
	   here @ -> a -> @ -> A -> + , so when reconstructing + -> A -> @ and still we need to
	   continue!
	   */
	while (stop != start || ring_of_keys != 0)
	{
		pair<pair<int, int>, int> par = parent[stop.first][stop.second][ring_of_keys];
		stop = par.first;
		ring_of_keys = par.second;
		ans.push_back(vector<int>(0));
		ans[ans.size() - 1].push_back(stop.first);
		ans[ans.size() - 1].push_back(stop.second);
	}
	reverse(ans.begin(), ans.end());
	return ans;
}
// node that start and stop are passed by reference hence change will be reflected.
void get_start_and_stop_positions(vector<string> &grid, pair<int, int> &start,
pair<int, int> &stop)
{
	int rows = grid.size();
	int cols = grid[0].length();
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			if (is_start(grid[r][c]))
			{
				start = {r, c};
			}
			else if(is_stop(grid[r][c]))
			{
				stop = {r, c};
			}
		}
	}
}
vector<vector<int>> find_shortest_path(vector<string> grid)
{
	int rows = grid.size();
	int cols = grid[0].length();
	pair<int, int> start, stop;
	// Get the starting and ending point.
	get_start_and_stop_positions(grid, start, stop);
	/*
	   dp[r][c][ring] denotes length of shortest path from starting point to (r, c) and we have
	   already got keys in the ring.
	   */
	cout<<"MAX_MASK = "<<MAX_MASK<<", INF = "<<INF<<endl;
	vector<vector<vector<int>>> dp(rows, vector<vector<int>>(cols,
				vector<int>(MAX_MASK, INF)));
	/*
	   parent[r][c][ring] denotes the last node through which dp[r][c][ring] is updated. This will
	   help to reconstruct the path.
	   */
	vector<vector<vector<pair<pair<int, int>, int>>>> parent(rows,
			vector<vector<pair<pair<int, int>, int>>>(cols, vector<pair<pair<int, int>, int>>
				(MAX_MASK, {{-1, -1}, -1})));
	/*
	   visited[r][c][ring] keeps track of dp[r][c][ring] is visited or not. Though parent is enough
	   to track this, but for readability purpose I have added this also.
	   */
	vector<vector<vector<bool>>> visited(rows,
			vector<vector<bool>>(cols, vector<bool>(MAX_MASK, false)));
	// Do bfs.
	bfs(grid, start, dp, parent, visited);
	/*
	   As dp[r][c][ring] denotes shortest path from start point and we have already got keys in
	   the
	   ring, we just need to find for which key path is shorter.
	   */
	int length = INF;
	int ring_of_keys;
	for (int i = 0; i < MAX_MASK; i++)
	{
		if (length > dp[stop.first][stop.second][i])
		{
			length = dp[stop.first][stop.second][i];
			ring_of_keys = i;
		}
	}
	// cout << length << endl;
	// Build the path from using parent.
	return build_path(dp, parent, ring_of_keys, start, stop);
}


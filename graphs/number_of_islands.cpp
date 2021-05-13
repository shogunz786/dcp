int row[8]={-1, -1, -1, 0, 1, 1,  1,  0};
int col[8]={-1,  0,  1, 1, 1, 0, -1, -1}

void bfs(int r, int c, vector<vector<int>> &matrix)
{
	queue<pair<int,int>> q;
	q.push(make_pair(r,c));
	matrix[r][c]=0;
	while(!q.empty())
	{
		r = q.front().first;
		c = q.front().second;
		q.pop();
		for(int i=0; i<8; i++)
		{
			if(r+row[i]<0 || r+row[i]>=matrix.size() || c+col[i]<0 || c+col[i]>=matrix[0].size())
				continue;
			if(matrix[r+row[i]][c+col[i]]){
				matrix[r+row[i]][c+col[i]]=0;
				q.push(make_pair(r+row[i],c+col[i]));
			}
		}
	}
}


int count_islands(vector<vector<int>> matrix)
{
	int islands=0;
	for(int i=0; i<matrix.size(); i++)
	{
		for(int j=0; j<matrix[0].size(); j++)
		{
			if(matrix[i][j]){
				islands++;
				bfs(i,j,matrix);
			}
		}
	}
	return islands;
}

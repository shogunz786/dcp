void bfs(int r, int c, vector<vector<int>> &matrix)
{
	int row[4]={-1, 0, 1, 0};//all 4 directions
	int col[4]={ 0, 1, 0, -1};
	queue<pair<int,int>> q;
	q.push(make_pair(r,c));
	matrix[r][c]=0;
	while(!q.empty())
	{
	    r = q.front().first;
	    c = q.front().second;
	    q.pop();
            for(int i=0; i<4; i++)
	    {
		    int nr = r+row[i];
		    int nc = c+col[i];
		    if(nr<0 || nr>=matrix.size() || 
		       nc<0 || nc>=matrix[0].size())
			    continue;
		    if(matrix[nr][nc]){
			    matrix[nr][nc]=0;
			    q.push(make_pair(nr,nc);
		    }
	    }
	}
}
int getNoClusters(vector<vector<int>> matrix)
{
	int clusters=0;
	for(int i=0; i<matrix.size(); i++)
	{
		for(int j=0;j<matrix[0].size(); j++)
		{
			if(matrix[i][j])
			{
				clusters++;//number of bfs runs possible
				bfs(i,j, matrix);
			}
		}
	}
	return clusters;
}

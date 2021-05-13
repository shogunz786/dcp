class Step{//track current location and distance
	public:
	int r;
	int c;
	int distance;
	Step(int i, int j, int w){ r=i; c=j; distance=w;}
};
void shortest_distance_from_guard(vector<vector<char>> &matrix)
{
	queue<Step> q;
	for(int i=0; i<matrix.size(); i++)
	{
		for(int j=0; j<matrix[0].size(); j++)
		{
			if(matrix[i][j]!='G' && matrix[i][j]!='O' && matrix[i][j]!='W')
				return;
			if(matrix[i][j]=='G')
				q.push(Step(i,j,0));//add guards to queue
		}
	}
	int row[4]={-1, 0, 1, 0};
	int col[4]={0, 1, 0, -1};
	while(!q.empty())
	{
		int r=q.front().r;
		int c=q.front().c;
		int d=q.front().distance;
		q.pop();
		for(int i=0; i<4; i++)
		{
			int nr = r+row[i];
			int nc = c+col[i];
			if(nr<0 || nc<0 || 
					nr>=matrix.size() || nc>=matrix[0].size())
				continue;
			if(matrix[nr][nc]=='G' || matrix[nr][nc]=='W')
				continue;
			if(matrix[nr][nc]=='O'){
				matrix[nr][nc]=(char)(d+1);
				q.push(Step(nr,nc,d+1));
			}else{
				int distance = (int)matrix[nr][nc];
				if(distance>d+1)
				{
					matrix[nr][nc]=(char)(d+1);
					q.push(Step(nr,nc,d+1));
				}
			}
		}
	}
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++){
			if(matrix[i][j]=='G' || matrix[i][j]=='W')
				cout << std::setw(3) << matrix[i][j];
			else
				cout << std::setw(3) << ((int)matrix[i][j]);
		}
		cout << endl;
	}
}

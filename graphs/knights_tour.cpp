struct Turn{
	int r;//row
	int c;//col
	int m;//moves
	Turn(int row, int col, int moves){r=row; c=col; m=moves;}
};
int find_min_num_of_moves(int rows, int cols, int s_row, int s_col, int e_row, int e_col)
{
	if(rows<=0 || cols<=0 ||
       	s_row>=rows || s_row<0 ||
       	s_col>=cols || s_col<0 ||
       	e_row>=rows || e_row<0 ||
       	e_col>=cols || e_col<0)
		return -1;
	vector<vector<bool>> visited(rows, vector<bool>(cols, false));
	int krow[8]={-2, -2, -1, 1, 2, 2, 1, -1};//possible knight moves
	int kcol[8]={-1, 1, 2, 2, 1, -1, -2, -2};
	int m = 0;
	bool reached_end=false;
	queue<Turn> q;
	Turn t(s_row, s_col, 0);
	q.push(t);
	visited[s_row][s_col]=true;
	while(!q.empty())
	{
		t = q.front();
		q.pop();
		if(t.r == e_row && t.c == e_col)
			return t.m;
		for(int i=0; i<8; i++)
		{
			int r=t.r+krow[i];
			int c=t.c+kcol[i];
			if(r<0 || c<0 || //move on the board
		       	r>=rows || c>=cols)
				continue;
			if(visited[r][c]==false)
			{
				Turn u(r,c,t.m+1);
				visited[r][c]=true;
				q.push(u);
			}
		}
	}
	return -1;
}

struct Step{
	public:
	int r;
	int c;
	Step(int i, int j){r=i; c=j}
};
int row[4] = {1, 0, -1, 0};
int col[4] = {0, 1, 0, -1};
Step findSink(int r, int c, vector<vector<int>> matrix){
	int minr=r, minc=c;
	for(int k=0; k<8; k++)
	{
		int nr = r+row[k];
		int nc = c+col[k];
		if(nr<0 || nc<0 || nr>=matrix.size()
				|| nc>=matrix[0].size())
			continue;
		if(matrix[minr][minc]>matrix[nr][nc]){
			minr=nr;
			minc=nc;
		}
	}
	return Step(minr,minc);
}

void rainfall_challenge(vector<vector<int>> matrix)
{
	vector<vector<char>> output(matrix.size(), vector<char>(matrix[0].size(),' '));
	unordered_map<char, int> mpcount;
	queue<Step> q;
	char marker='A';
	for(int j=0;j<matrix.size(); j++)
	{
		for(int k=0;k<matrix[0].size(); k++)
		{
			if(output[j][k]==' ')//only add unmarked cells to queue
				q.push(Step(j,k));
			while(!q.empty())
			{
				Step unmarkedStep=q.front();
				q.pop();
				Step sinkStep = findSink(unmarkedStep.r,unmarkedStep.c, matrix);
				if(sinkStep.r==unmarkedStep.r && 
				sinkStep.c==unmarkedStep.c && 
				output[unmarkedStep.r][unmarkedStep.c]==' '){//current node is a sink and unmarked
					output[unmarkedStep.r][unmarkedStep.c]=marker;//add marker and keep count
					mpcount.insert(make_pair(marker,1));
					marker=(char)(marker+1);
				}
				for(int i=0; i<4; i++)//examine neighbors
				{
					int nr = unmarkedStep.r+row[i];
					int nc = unmarkedStep.c+col[i];
					if(nr<0 ||
							nc<0 || 
							nr>=matrix.size() ||
							nc>=matrix[0].size())
						continue;
					if(matrix[nr][nc]<matrix[unmarkedStep.r][unmarkedStep.c]){//check if smaller neighbor
						if(output[nr][nc]==' ')
						{
							q.push(Step(nr,nc)); //first figure out the smaller node so break
							break;
						}else{
							if(sinkStep.r==nr && sinkStep.c==nc)//this neighbor is our sink
							{
								if(output[unmarkedStep.r][unmarkedStep.c]==' ')
									q.push(Step(nr,nc));//reexamine the neighbors as its neighbor is sink but it has unmarked neighbors
								else
									mpcount[output[unmarkedStep.r][unmarkedStep.c]]--;
								output[unmarkedStep.r][unmarkedStep.c]=output[nr][nc];//update current cell with its sink
								mpcount[output[unmarkedStep.r][unmarkedStep.c]]++;
							}
						}
					}else{
						//i may be your sink ?
						if(output[nr][nc]!=output[unmarkedStep.r][unmarkedStep.c]
					       	&& output[unmarkedStep.r][unmarkedStep.c]!=' '){//if this is the sink then own it
							Step thisStep = findSink(nr,nc,matrix);//find neighbors sink
							if(thisStep.r==unmarkedStep.r &&
						       	thisStep.c==unmarkedStep.c)
							{
								if(output[nr][nc]==' ')
									q.push(Step(nr,nc));//reexamine the neighbors as we just populate it
								else
									mpcount[output[nr][nc]]--;
								output[nr][nc]=output[unmarkedStep.r][unmarkedStep.c];
								mpcount[output[unmarkedStep.r][unmarkedStep.c]]++;
							}
						}
					}
				}
			}
		}
	}
	for(int i=0; i<output.size(); i++)
	{
		for(int j=0; j<output.size(); j++)
		{
			cout<<output[i][j]<<" ";
		}
		cout<<endl;
	}
	map<char,int>::iterator iter=mpcount.begin();
	for(;iter!=mpcount.end();iter++)
	{
		cout<<iter->first<<" = "<<iter->second<<endl;
	}
}

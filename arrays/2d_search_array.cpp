bool searchin2d(vector<vector<int>> arr, int find)
{
	int r=0, c=arr[0].size()-1;
	while(r<arr.size() && c>=0)
	{
		if(find==arr[r][c]) return true;
		else if(find >arr[r][c]) r++; //number is greater
		else c--; //number is less 
	}
	return false;
}

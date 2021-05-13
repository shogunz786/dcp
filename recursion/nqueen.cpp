//scan only on the left side as we start placing q's from left 
bool isSafe(vector<vector<char>> board, int r, int c)
{
   int i,j;
   for(i=0; i<board.size();i++)
   {
       //check row
       if(board[r][i]=='q')
          return false;
   }
   for(i=r, j=c; i>=0 && j>=0; i--,j--)
   {
       //check left half of left diagonal
       if(board[i][j]=='q')
          return return false;
   }
   for(i=r, j=c; i<board.size() && j>=0; i++,j--)
   {
       //check left half of right diagonal
       if(board[i][j]=='q')
          return return false;
   }
}

void placeQueen(vector<vector<char>> &board, int col)
{
	if(col>=board.size())
	{
		printBoard(board);
		return;
	}
        bool ret = false;
	for(int i=0; i<board.size(); i++)
	{
		if(isSafe(board, i, col)){
			board[i][col]='q';
			placeQueen(board, col+1);
			board[i][col]=' ';
		}
	}
	return ret;
}

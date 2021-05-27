/*
Given height h=3 and width w=4
. . . .
. . . .
. . . .

Find number of ways to reach from top left point to
bottom right corner moving only down or right.
*/

using namespace std;

//get top or left allowed neighbor
int getNeighborSum(vector<vector<int>> &dp, int r, int c){
    int topR=r-1, leftR=r;
	  int topC=c, leftC=c-1;
	  int sum=0;
	  if(topR>=0){
			sum+=dp[topR][topC];
		}
	  if(leftC>=0){
			sum+=dp[leftR][leftC];
		}
	  return sum;
}
//time O(nm) and space O(nm)
int numberOfWaysToTraverseGraphDP(int width, int height) {
	vector<vector<int>> dp(height,vector<int>(width,0));
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			//all left and top points has only one way
			if(i==0 || j==0){
	       dp[i][j]=1;
			}else{
	       dp[i][j]=getNeighborSum(dp, i, j);
			}
		}
	}
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			cout<<i<<","<<j<<"=="<<dp[i][j]<<" ";
		}
		cout<<endl;
	}
  return dp[height-1][width-1];
}

//O(2^(n+m)) time and O(n+m) space
int numberOfWaysToTraverseGraphRec(int width, int height) {
	if(width==1 or height==1){
		return 1;
	}
	//bottom up recursion
	//since ways to reach width & height location is
	//sum of ways to reach width-1 & height and width and height-1
	return numberOfWaysToTraverseGraphRec(width-1,height)+
		     numberOfWaysToTraverseGraphRec(width,height-1);
}
int factorial(int n)
{
	int f=1;
	for(int i=2;i<=n;i++){
		f*=i;
	}
	return f;
}
//O(n+m) time and O(1) space
int numberOfWaysToTraverseGraph(int width, int height) {
	int xFromCorner = width-1;
	int yFromCorner = height-1;
	//number of permuations of right and down movements
	//is the number of ways to reach the bottom right corner
	//generate all the possible permutations
	//say for height 3 and width 4 to reach from
	//top left to bottom right
	//e.g: RRRDD
	//     DRRRD
	// use (R+D)!/(R!*D!)
	return int(factorial(xFromCorner+yFromCorner)/
						 (factorial(xFromCorner)*factorial(yFromCorner)));
}

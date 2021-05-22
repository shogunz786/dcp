/*
  given height and maxstep 1 to n allowed
	find maximum ways to reach height
*/
using namespace std;

//O(n*k) time and O(n) space - n is height and k is number of maxsteps
int staircaseTraversalDP(int height, int maxSteps) {
  //init dp with height+1 sized array
	vector<int> steps(height+1,0);
  //0 or 1 height can be reached in 1 number of ways
	steps[0]=1;
	steps[1]=1;
	for(int i=2; i<= height; i++){
		for(int j=1; j<=min(maxSteps,i); j++){
      //if height is greater than steps
		  if(i>=j){
        //add the number of ways taken to reach height - step
			  steps[i]+=steps[i-j];
		  }
		}
	}
  return steps[height];
}

//O(n^k) time and O(n) space - n is height and k is number of steps
int helper(int height, int maxSteps){
  //if height is <= 1 number of ways is 1
	if(height<=1){
		return 1;
	}
	int noWays = 0;
	for(int i=1; i<min(maxSteps,height)+1; i++){
      //for each number of steps reduce the height and retry recusively
		  noWays+=helper(height-i,maxSteps);
	}
	return noWays;
}


int staircaseTraversalRec(int height, int maxSteps){
	return helper(height, maxSteps);
}

int helperMemozation(unordered_map<int,int> &mp,int height, int maxSteps){
	if(height<=1){
		return 1;
	}
	if(mp.find(height)!=mp.end()){
		return mp[height];
	}
	int noWays = 0;
	for(int i=1; i<min(maxSteps,height)+1; i++){
		  noWays+=helper(height-i,maxSteps);
	}
  //memoize along with recursion
  mp[height]=noWays;	
	return noWays;
}
int staircaseTraversal(int height, int maxSteps){
	unordered_map<int,int> mp={{0,1},{1,1}};
	return helperMemozation(mp, height, maxSteps);
}

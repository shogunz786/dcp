#include <vector>
using namespace std;

//compute gcd
int getGCD(int a, int b){
	int t=0;
	while(true){
		if(a==0)
			return b;
		if(b==0)
			return a;
		t=a%b;
		a=b;
		b=t;
	}
}
//O(n^2) time and space O(n) - n is number of points
int lineThroughPoints(vector<vector<int>> points) {
  int maxPoints = 1;
	string key;
	//for each point check if rest of the points are on the line
	for(int i=0; i<points.size(); i++){
		//keep track of how many point pairs have same slope
		unordered_map<string,int> sameSlopeCount;
		for(int j=i+1; j<points.size(); j++){
			//ignore duplicate points
			if(points[i]!=points[j]){
				//x delta
				int run = points[i][0]-points[j][0];
				//y delta
				int rise = points[i][1]-points[j][1];
				//find gcd to reduce the slope
				int gcd = getGCD(abs(run),abs(rise));
        //reduce run and rise
				run=run/gcd;
				rise=rise/gcd;
				//make sure -ve value is captured in rise
				//to be consistent across all slope
				//computations
				if(run<0){
					run*=-1;
					rise*=-1;
				}
				//compute key to track slopes
				key=to_string(rise)+":"+to_string(run);
        //keep track of point pairs with same slope
			  if(sameSlopeCount.find(key)==sameSlopeCount.end()){
				  sameSlopeCount.insert({key,1});
			  }
			  sameSlopeCount[key]++;
				//keep track of the maxpoints with same slope
			  maxPoints=max(maxPoints,sameSlopeCount[key]);
			}
		}
	}
	
  return maxPoints;
}


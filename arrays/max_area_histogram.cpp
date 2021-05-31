

/*
 Given building heights [1, 3, 3, 2, 4, 1, 5, 3, 2]
 find max area rectangles that can be formed, a rectagle 
 can be formed by number of buildings of greater or equal
 contigous buildings * the height of the current building
 
 result:9
 */
#include <vector>
#include <stack>
using namespace std;
//O(n^2) time and O(1) space
int largestRectangleUnderSkylineBF(vector<int> buildings) {
  //for each index scan left and right to find the max number
	//of adjacent buildings that are greater or equal to current
	//index
	int area=0;
	for(int i=0; i<buildings.size(); i++){
		//scan left
		int l=i-1;
		while(l>=0 && buildings[i]<=buildings[l]){
			l--;
		}
		//scan right
		int r=i+1;
		while(r<buildings.size() && buildings[i]<=buildings[r]){
			r++;
		}
		//capture max area
		area=max(area,(r-l-1)*buildings[i]);
	}
  return area;
}

//O(n) time and O(n) space
int largestRectangleUnderSkyline(vector<int> buildings) {
	stack<int> s;
	
	int i=0;
	int maxArea=0;
	int len=0;
	int top=0;
	while(i<buildings.size()){
		//push into stack if current building height
		//in top of the stack is less that current
		//building height being examined
		if(s.empty() || buildings[i]>=buildings[s.top()]){
		   s.push(i++);	
		}else{
			 //if current building height being examined is smaller
			 //than top of the stack's building height
			 //examine max area possible with current top of stack building
			 //and remove it from stack
			 //the length will be current  index - top of stack
			 //if stack empty the length will be current index
			 //which means all the previos buildings are greater than
			 //current index
		   top=s.top();
			 s.pop();
			 
			 len=s.empty()?i:i-top;
			 maxArea=max(maxArea,len*buildings[top]);
		}
	}
	//process remaining elements in the stack as before
	while(!s.empty()){
		   top=s.top();
			 s.pop();
			 len=s.empty()?i:i-top;
			 maxArea=max(maxArea,len*buildings[top]);
	}
	return maxArea;
}

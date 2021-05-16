/*
 * Given building heights find the largest triangle that can be drawn along
 * the building shapes.
 */
#include <vector>
#include <stack>
#include <iostream>
using namespace std;

//O(n) time and O(n) space - n number of buildings
int largestRectangleUnderSkyline(vector<int> buildings) {
	stack<int> pillarIdxs;
	int mxArea=0;
	vector<int> extBuildings(buildings);
	//add 0 height at the end
	extBuildings.push_back(0);
	for (int i=0; i<extBuildings.size(); i++){
		//get current height
		int ht=extBuildings[i];
		//if current building height is less than the building height
		//on top of the stack then a rectagle of same height can't be extended
		//so capture the area using building height on
		//top of stack and find width using current index and 
		//index of previous top of stack
		//calculate area and keep track of max area
		//only calculate rectangles on the left side
		//e.g: 1 3 3 2 4 1 5 3 2
		//    keep adding index of increasing heights
		//     1(0) 3(1) 3(2) 
		// now 2(3) is less than 2(3)
		// remove 3(2) from index stack
		// ht=3(2) top of index stack now is 3(1)
		//so width=curr index 2(3) - top of stack index 3(1) - 1 
		// capture height and repeat till curr height is greater than top of index stack
		//then add 2(3) to index stack and repeat
		while(pillarIdxs.size()!=0 && 
				 extBuildings[pillarIdxs.top()] > ht){
			int pHt = extBuildings[pillarIdxs.top()];
			pillarIdxs.pop();
			int w = pillarIdxs.size()==0? i: i-pillarIdxs.top()-1;
			mxArea = max(w*pHt, mxArea);
		}
		pillarIdxs.push(i);
	}
  return mxArea;
}

//O(n^2) time and O(1) space
int largestRectangleUnderSkylineBruteForce(vector<int> buildings) {
	int maxArea=0;
	for(int i=0; i<buildings.size(); i++){
		//go left till u find smaller building
		int left=0;
		int j=i-1;
		while(j>=0){
			if(buildings[i]>buildings[j]){
				left=j.
				break;
			}
			j--;
		}
		//go right till u find smaller building
		j=i+1;
		right=buildings.size()-1;;
		while(j<buildings.size()){
			if(buildings[i]>buildings[j]){
				right=j.
				break;
			}
			j++;
		}
		maxArea=max(maxArea,(right-left-1)*buildings[i]);
	}
	return maxArea;
}

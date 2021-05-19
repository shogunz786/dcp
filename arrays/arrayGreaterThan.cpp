/*
 Given two arrays
 {
  "blueShirtHeights": [6, 9, 2, 4, 5],
  "redShirtHeights": [5, 8, 1, 3, 4]
}
 check if one of the array has all numbers greater than the second array.
 result:
 true
 */
#include <vector>
using namespace std;

bool classPhotos(vector<int> redShirtHeights, vector<int> blueShirtHeights) {
	bool red = false;//red can't be in the back
	sort(redShirtHeights.begin(),redShirtHeights.end());
	sort(blueShirtHeights.begin(),blueShirtHeights.end());
	if(redShirtHeights[0]>blueShirtHeights[0]){
		red=true;
	}
	for(int i=0; i<redShirtHeights.size(); i++){
	  if(red && redShirtHeights[i]<=blueShirtHeights[i]){
		   return false;	
		}else	 if(!red && redShirtHeights[i]>=blueShirtHeights[i]){
		   return false;	
		}	
	}
  return true;
}

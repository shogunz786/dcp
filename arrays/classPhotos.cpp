#include <vector>
using namespace std;

//O(nlog(n)) time and O(1) time
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

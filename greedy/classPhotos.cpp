#include <vector>
using namespace std;

//O(nlogn) time and O(1) space
bool classPhotos(vector<int> red, vector<int> blue) {
  sort(red.begin(), red.end());
  sort(blue.begin(), blue.end());
  bool redfront =true;
  bool bluefront=true;
  for(int i=0; i<red.size() && (redfront || bluefront); i++){
    if(red[i]<=blue[i] && redfront){
      redfront = false;
    }
    if(blue[i]<=red[i] && bluefront){
      bluefront = false;
    }
  }
  return redfront || bluefront;
}
/*
1,3,4,5,8
2,4,5,6,9
*/

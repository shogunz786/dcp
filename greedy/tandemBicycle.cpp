#include <vector>
using namespace std;

//O(nlogn) time and O(1) space
int tandemBicycle(vector<int> red, vector<int> blue,
                  bool fastest) {
  int speed = 0;
  sort(red.begin(), red.end());
  sort(blue.begin(), blue.end());
  if(!fastest){
    for(int i=0;i<red.size(); i++){
      if(red[i]>blue[i]){
        speed+=red[i];
      }else{
        speed+=blue[i];
      }
    }
  }else{
    int j=0;
    for(int i=red.size()-1;i>=0; i--){
      if(red[i]>blue[j]){
        speed+=red[i];
      }else{
        speed+=blue[j];
      }
      j++;
    }
  }
  return speed;
}

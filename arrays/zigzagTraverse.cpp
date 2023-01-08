#include <vector>

using namespace std;

vector<int> zigzagTraverse(vector<vector<int>> array) {
  int r=0, c=0;
  vector<int> res;
  bool goingDown = true;
  while(r>=0 && c>=0 && r<array.size() && c<array[0].size()){
    res.push_back(array[r][c]);
    if(goingDown){
      if(c==0 || //reach left most
        r==array.size()-1){//reach bottom most
        goingDown = false;//change direction
        if(r==array.size()-1){
          c++;//when at bottom go to next col
        }else{
          r++;
        }
      }else{//go down
        c--;
        r++;
      }
    }else{
      if(r==0 || //reach top most
        c==array[0].size()-1){//reach right most
        goingDown = true;//change direction
        if(c==array[0].size()-1){
          r++;//when at right most go to next row
        }else{
          c++;
        }
      }else{//go up
        r--;
        c++;
      }
    }
  }
  return res;
}

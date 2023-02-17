#include <vector>
using namespace std;

set<string> getKeys(vector<vector<int>> &points){
  set<string> keys;
  for(auto v: points){
    keys.insert(to_string(v[0])+":"+to_string(v[1]));
  }
  return keys;
}
int minimumAreaRectangle(vector<vector<int>> points) {
  set<string> keys = getKeys(points); 
  int minArea = INT_MAX;
  //find all possible diagonals
  for(int i=0; i<points.size(); i++){
    for(int j=0; j<i; j++){
      //do not consider diagonals 
      //parallel to x or y axis
      //if same x parallel to x axis or
      //same y parallel to y axis
      if(points[i][0]==points[j][0] ||
        points[i][1]==points[j][1]){
          continue;
      }
      //       p1 x1,y2 *--* x2,y2 points[j]
      //                  /
      //                 /
      //points[i] x1,y1 *--* x2,y2 p2
      
      int p1 = keys.count(to_string(points[i][0])+":"+to_string(points[j][1]));
      int p2 = keys.count(to_string(points[j][0])+":"+to_string(points[i][1]));

      if(p1 && p2){
        int area = abs(points[i][0]-points[j][0])*abs(points[i][1]-points[j][1]);
        minArea=min(minArea, area);
      }
    }
  }
  return minArea == INT_MAX ? 0: minArea;
}

#include <vector>
using namespace std;

//b) a
//  -b
// ----
//   rem)b
int gcd(int a, int b){
  int rem = 0;
  while(true){
    //find reminder till either a or b is zero
    if(a==0){
      return b;
    }else if(b==0){
      return a;
    }
    rem = a%b;
    a = b;
    b = rem;
  }
}

string getSlopeKey(vector<int> &a, vector<int> &b){
  int xdiff = a[0]-b[0];
  int ydiff = a[1]-b[1];
  //points on same vertical line
  if(xdiff==0){
    ydiff = 1;
  }else if(ydiff==0){
    //points in same horizontal line
    xdiff = 1;
  }else{
    int g = gcd(abs(xdiff), abs(ydiff));
    //reduce to find rate of rise
    xdiff /= g;
    ydiff /= g;

    //normalize sign if xdiff<0
    if(xdiff<0){
      xdiff *= -1;
      ydiff *= -1;
    }
  }

  return to_string(ydiff)+":"+to_string(xdiff);
}
//time O(n^2) and space O(n)
int lineThroughPoints(vector<vector<int>> points) {
  string slopeKey;
  //when there is only one point 
  int maxPoints = 1;
  for(int i=0; i<points.size()-1; i++){
    //capture a line that goes thru points[i] and any other points 
    unordered_map<string, int> pointWithSameSlope;
    for(int j=i+1; j<points.size(); j++){
      //find slope
      slopeKey = getSlopeKey(points[i], points[j]);
      if(pointWithSameSlope.count(slopeKey)==0){
        //initially two points to make a line 
        pointWithSameSlope[slopeKey]=2;
      }else{
         //increment for additional points
         pointWithSameSlope[slopeKey]++;
      }

      //capture max points with same slope
      maxPoints = max(maxPoints, pointWithSameSlope[slopeKey]);
    }
  }
  return maxPoints;
}

#include <vector>
using namespace std;

//O(n^2) time and O(1) space
int validStartingCity(vector<int> distances, vector<int> fuel, int mpg) {

  for(int i=0;i<fuel.size(); i++){
    fuel[i]*=mpg;
  }
  for(int j=0; j<fuel.size(); j++){
      int miles = fuel[j];
      int distance = distances[j];
      int moreMiles = miles;
      int milesRemaining = moreMiles - distance;
      //cout<<"START "<<j<<") "<<miles<<" - "<<moreMiles<<" - "<<distance<<" = "<<milesRemaining<<endl;
 
      if(milesRemaining<0){
        continue;
      }
     for(int i=j+1; i<2*fuel.size(); i++){
          int index = i%fuel.size();
          if(index==j){
            //cout<<" found "<<j<<endl;
            return j;
          }
          miles = fuel[index];
          distance = distances[index];
          moreMiles=milesRemaining+miles;
          milesRemaining=moreMiles-distance;
          //cout<<" "<<index<<") "<<miles<<" - "<<moreMiles<<" - "<<distance<<" = "<<milesRemaining<<endl;
          if(milesRemaining<0){
            //cout<<" insufficient miles "<<milesRemaining<<endl;
            break;
          }
      }
  }
  return -1;
}


//O(n) time and O(1) space
int validStartingCity(vector<int> distances, vector<int> fuel, int mpg) {
  // Write your code here.
  int remainingMiles = 0;
  int milesRemainingAtStartingCityCandidate = 0;
  int j = 0;
  
  for(int i=1; i<fuel.size(); i++){
    int distance = distances[i-1];
    int miles = fuel[i-1]*mpg;
    remainingMiles += miles - distance;

    if(remainingMiles < milesRemainingAtStartingCityCandidate){
      cout<<j<<"))) "<<miles<<" - "<<distance<<" = "<<remainingMiles<<" <? "<<milesRemainingAtStartingCityCandidate<<endl;
      milesRemainingAtStartingCityCandidate = remainingMiles;
      j = i;
    }else{
      cout<<j<<") "<<miles<<" - "<<distance<<" = "<<remainingMiles<<" <? "<<milesRemainingAtStartingCityCandidate<<endl;
    }
  }
  return j;
}

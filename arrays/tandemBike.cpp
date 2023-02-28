/*
 *input:{
  "blueShirtSpeeds": [3, 6, 7, 2, 1],
  "fastest": true,
  "redShirtSpeeds": [5, 5, 3, 9, 2]
  }
	given two arrays, find matching tandem riders with fastest total speed
	when rider a and b with different speed ride a tandem bike the bike
	speed will be of the fastest rider.
	if fastest true find after pairing find the total max speed possible.
	if not fastest find after pairing the total minimum speed
	*/
#include <vector>
using namespace std;

//O(nlog(n)) time and O(1) space - n number of bikes
int tandemBicycle(vector<int> redShirtSpeeds, vector<int> blueShirtSpeeds,
                  bool fastest) {
	//sort blue riders 
	sort(blueShirtSpeeds.begin(),blueShirtSpeeds.end());
	if(!fastest){
		 //sort red riders in desc order
	   sort(redShirtSpeeds.begin(),redShirtSpeeds.end(),greater<int>());
        }else{
		 //sort red riders 
	   sort(redShirtSpeeds.begin(),redShirtSpeeds.end());
        }
	int total=0;
	for(int i=0; i<redShirtSpeeds.size(); i++){
		int  r = redShirtSpeeds[i];
		int b = blueShirtSpeeds[blueShirtSpeeds.size()-1-i];
		//match first and last rider
		//in case of fastest match slow red rider with fast blue rider
		//in cse of slowest match slow red rider with slow blue rider
		total+=max(r,b);
	}
        return total;
}


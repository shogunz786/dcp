/*
 Given blocks and their proximity to reqs. Find a block which is conviniently located
 close to all reqs.
 "blocks": [
    {
      "gym": false,
      "school": true,
      "store": false
    },
    {
      "gym": true,
      "school": false,
      "store": false
    },
    {
      "gym": true,
      "school": true,
      "store": false
    },
    {
      "gym": false,
      "school": true,
      "store": false
    },
    {
      "gym": false,
      "school": true,
      "store": true
    }
  ],
  "reqs": ["gym", "school", "store"]
	
	find block which has all the reqs at least distance.
*/
#include <vector>
#include <unordered_map>

using namespace std;

//O(b^2*r) time and O(b) space b - blocks r-requirements
int apartmentHunting(vector<unordered_map<string, bool>> blocks,
                     vector<string> reqs) {
	//keep track of max distances across reqs at each block
	vector<int> maxDisAtBlock(blocks.size(), INT_MIN);
	for(int i=0; i<blocks.size(); i++){
		//for each block check the reqs
		for(auto req:reqs){
		   int closeReqDis = INT_MAX;
			 //for this block i find the closest block in distance
		   for(int j=0; j<blocks.size(); j++){
			   if(blocks[j][req]){
					//keep track lowest distance from block i to j
					closeReqDis = min(closeReqDis,abs(i-j)); 
				 }
		   }
			 //keep track of the max distances for any req at this block
			 maxDisAtBlock[i]=max(maxDisAtBlock[i],closeReqDis);
		}
	}
  //find least max distance for any req in all blocks
	int theMinBlockIdx = 0;
	int theMinVal = INT_MAX;
	for(int i=0; i<blocks.size(); i++){
	   if(maxDisAtBlock[i]<theMinVal){
			 theMinVal=maxDisAtBlock[i];
			 theMinBlockIdx=i;
		 }	
	}
  return theMinBlockIdx;
}

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
#include <climits>
#include <algorithm>
#include <cmath>

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

vector<int> getMinDis(vector<unordered_map<string,bool>> blocks, string req){
	vector<int> minDis(blocks.size());
	int closeReqIdx = INT_MAX;
	//find the closest location from left side
	for(int i=0; i<blocks.size(); i++){
		if(blocks[i][req])
			closeReqIdx = i;
		minDis[i]=abs(i-closeReqIdx);
	}
	//find the closest from the right side
	//get closest of left and right
	for(int i=blocks.size()-1; i>=0; i--){
		if(blocks[i][req])
			closeReqIdx=i;
		minDis[i]=min(minDis[i], abs(i-closeReqIdx));
	}
	return minDis;//minDis[blocks]
}

//find the block index of the min Value
//find mins of all maxs
int getIdxAtMinVal(vector<int> array){
	int idxAtMinVal=0;
	int minVal=INT_MAX;
	for(int i=0; i<array.size(); i++){
		if(array[i]<minVal){
			minVal=array[i];
			idxAtMinVal=i;
		}
	}
	return idxAtMinVal;
}

//convert array[req][blocks] to array[blocks][req] and find the max of all the reqs closest value in a block
vector<int> getMaxDisAtBlocks(vector<unordered_map<string,bool>> blocks, //blocks[blockIdx][req:true, req1:false]
				vector<vector<int>> minDisFromBlocks //minDisFromBlocks[req][blocks]
			     ){
	vector<int> maxDisAtBlocks(blocks.size());
	for(int i=0; i<blocks.size(); i++){
		int mx = INT_MIN;
		for(vector<int> dis: minDisFromBlocks)//minDisAtBlocks[req][blocks]
		{
			mx = max(mx, dis[i]);//collect max closest of all reqs in block
		}
		//find max of all the closest of all reqs
		maxDisAtBlocks[i]=mx;
	}
	return maxDisAtBlocks;
}

//O(br) time and O(br) space - b-number of blocks and r is number of reqs
int apartmentHunting2(vector<unordered_map<string, bool>> blocks,
                     vector<string> reqs) {
	//minDisFromBlocks[req][blocks]
	vector<vector<int>> minDisFromBlocks;
	//for each req find the minDisFromBlocks
	for(auto req: reqs){
		//get min dis for each req
		//minDisFromBlocks[req][blocks]
		minDisFromBlocks.push_back(getMinDis(blocks, req));
	}
	vector<int> maxDisAtBlocks = getMaxDisAtBlocks(blocks, minDisFromBlocks);
  return getIdxAtMinVal(maxDisAtBlocks);
}

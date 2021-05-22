/*
 given a matrix, convert -ve elements to positive if at least 
 one of the top, bottom, right or left neighbor is positive
 Find number of passes to convert, if it can't be converted 
 return -1.
 input:
 [
  [0, -1, -3, 2, 0],
  [1, -2, -5, -1, -3],
  [3, 0, 0, -4, -1]
]
output: 3
*/
#include <vector>
using namespace std;

void convertNegativeToPositiveAndAddToQueue(vector<vector<int>> &matrix,
											 int r, int c,
											 queue<vector<int>> &positives){
	int row[4]={-1,1,0,0};
	int col[4]={0,0,-1,1};
	
	for(int i=0; i<4; i++){
		int newr=r+row[i];
		int newc=c+col[i];
		if(newr<0 || newr>=matrix.size() ||
			 newc<0 || newc>=matrix[0].size()){
			continue;
		}
		if(matrix[newr][newc]<0){
			    //convert negative neighbor to positive and add to the queue
			    matrix[newr][newc]*=-1;
			    positives.push({newr,newc});
		}
	}
}

int convertToPositives(vector<vector<int>> &matrix,
											 queue<vector<int>> &positives){
 int passes=0;
 while(positives.size()){
  queue<vector<int>> q;
  while(positives.size()){
	  vector<int> v = positives.front();
	  positives.pop();
		//convert negative neighbors to positive and add to temporary queue
	  convertNegativeToPositiveAndAddToQueue(matrix,v[0], v[1],q);
	 }
	 passes++;
	 while(q.size()){
	 	 vector<int> v = q.front();
		 q.pop();
		 //move captured elements to global queue
		 positives.push(v);
	 }
 }
	return passes;
}

bool hasNegative(vector<vector<int>> &matrix){
	for(int i=0; i<matrix.size(); i++){
		for(int j=0; j<matrix[0].size(); j++){
			 if(matrix[i][j]<0){
				 //return true if there is a negative element
				 return true;
			 }
		}
	}
  return false;	
}

//O(w*h) time and O(w*h) space - w width of matrix and h height of matrix
int minimumPassesOfMatrix(vector<vector<int>> matrix) {
	queue<vector<int>> positives;
	
	for(int i=0; i<matrix.size(); i++){
		for(int j=0; j<matrix[0].size(); j++){
			 if(matrix[i][j]>0){
				 //collect all positives element locations in the matrix
				 positives.push({i,j});
			 }
		}
	}
	//convert negatives to positive
  int passes = convertToPositives(matrix, positives);
	//check if there are negatives
	return hasNegative(matrix)?-1:passes-1;
}


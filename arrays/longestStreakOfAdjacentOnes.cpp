/*
  Given   "array": [1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1]
	Find the index when replaced with 1 makes longest streak of 1's
	*/
using namespace std;

//O(n) time and O(1) time
int longestStreakOfAdjacentOnes(vector<int> array) {
	int longLen=0;
	int replaceIdx=-1;
	int longReplaceIdx=0;
	int curLen=0;
	
	for(int i=0;i<array.size(); i++){
		if(array[i]){
			curLen++;
		}else{
			//when we replace this index with 1 this is curlen
			curLen=i-replaceIdx;
			replaceIdx=i;
		}
		if(longLen<curLen){
			longLen=curLen;
			longReplaceIdx=replaceIdx;
		}
	}
  return longReplaceIdx;
}


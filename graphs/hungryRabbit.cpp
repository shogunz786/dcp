#include<vector>
#include<iostream>

using namespace std;

vector<vector<int>> neighbors = {
	{-1, 0},
	{1, 0},
	{0, -1},
	{0, 1}
};

bool isValidLocation(vector<vector<int>> &array, int r, int c){
	return r<array.size() && c<array[0].size();
}

vector<int> pickNext(vector<vector<int>> &array, int row, int col){
	int mxCarrots = INT_MIN;
	vector<int> mxLoc;
	for(auto loc: neighbors) {
		int r = row+loc[0];
		int c = col+loc[1];
		if(isValidLocation(array, r, c)){
			if(mxCarrots<array[r][c] && array[r][c]>0){
				mxCarrots = array[r][c];
				mxLoc = vector<int>{r, c};
			}
		}
	}
	return mxLoc;
}


vector<int> pickCenter(vector<vector<int>> &array){
	int rmid = array.size()/2;
	int cmid = array[0].size()/2;

	int mxCarrots = array[rmid][cmid];
	vector<int> mxLoc = {rmid, cmid};

	for(auto loc: neighbors) {
		int r = rmid+loc[0];
		int c = cmid+loc[1];
		if(isValidLocation(array, r, c)){
			if(mxCarrots<array[r][c] && array[r][c]>0){
				mxCarrots = array[r][c];
				mxLoc = vector<int>{r, c};
			}
		}
	}
	return mxLoc;
}

void printElement(vector<vector<int>> &array, vector<int> element){

	cout<<"array["<<element[0]<<"]["<<element[1]<<"]="<<array[element[0]][element[1]]<<endl;
}

int gatherCarrots(vector<vector<int>> &array){
	if(array.size()==0 || array[0].size()==0){
		return 0;
	}
	int count = 0;
	
	vector<int> center = pickCenter(array);

	vector<vector<int>> path;
	path.push_back(center);

	while(path.size()){
		vector<int> loc = path.back();
		//printElement(array, loc);
		path.pop_back();
		count += array[loc[0]][loc[1]];
		vector<int> nloc = pickNext(array, loc[0], loc[1]);
		array[loc[0]][loc[1]] = -1;
		if(nloc.size()!=0){
			path.push_back(nloc);
		}
	}
	return count;
}

int main(int argc, char *argv[]) {
	vector<vector<int>> array
	{  {5, 7, 8, 6, 3},
	   {0, 0, 7, 0, 4},
	   {4, 6, 3, 4, 9},
	   {3, 1, 0, 5, 8}
	};

	cout<<"carrots = "<<gatherCarrots(array)<<endl;

}

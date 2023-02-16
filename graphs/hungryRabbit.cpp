#include<vector>
#include<iostream>

using namespace std;

vector<vector<int>> neighbors = {
	{-1, 0},
	{1, 0},
	{0, -1},
	{0, 1}
};

void printElement(vector<vector<int>> &array, vector<int> element){

	cout<<"array["<<element[0]<<"]["<<element[1]
	<<"]="<<array[element[0]][element[1]]<<endl;
}

bool isValidLocation(vector<vector<int>> &array, int r, int c){
	return r>=0 && c>=0 && r<array.size() && c<array[0].size();
}

vector<int> pickNext(vector<vector<int>> &array, vector<int> element, int mxCarrots=INT_MIN){
	vector<int> mxLoc;
	for(auto loc: neighbors) {
		int r = element[0]+loc[0];
		int c = element[1]+loc[1];
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

	vector<int> row_options = {rmid, rmid};
	vector<int> col_options = {cmid, cmid};
	
	//when even more than one center
	if(array.size()%2==0){
		row_options[0]-=1;
	}
	//when even more than one center
        if(array[0].size()%2==0){
		col_options[0]-=1;
	}
	vector<int> cur, mxLoc;
	int mxCarrots = INT_MIN;
	int carrots;
	//iterate all centers for max
	for(auto i: row_options){
	   for(auto j: col_options){
	      cur = vector<int>{i,j};
	      carrots = array[cur[0]][cur[1]];
              if(mxCarrots<carrots){
		      mxCarrots = carrots;
		      mxLoc = cur;
	      }
	   }
	}
	return mxLoc;
}

int gatherCarrots(vector<vector<int>> &array){
	if(array.size()==0 || array[0].size()==0){
		return 0;
	}
	int count = 0;
	
	vector<int> center = pickCenter(array);

	vector<vector<int>> path;
	//start from center
	path.push_back(center);

	vector<int> loc, nloc;
	while(path.size()){
		loc = path.back();
		path.pop_back();
		count += array[loc[0]][loc[1]];
		//find next max element
		nloc = pickNext(array, loc);
		//mark visited
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

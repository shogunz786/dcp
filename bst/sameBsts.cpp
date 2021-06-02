#include <vector>

using namespace std;

vector<int> getSmaller(vector<int> arr){
	vector<int> res;
	for(int i=1; i<arr.size(); i++){
		if(arr[i]<arr[0])
			res.push_back(arr[i]);
	}
	return res;
}

vector<int> getBiggerOrEqual(vector<int> arr){
	vector<int> res;
	for(int i=1; i<arr.size(); i++){
		if(arr[i]>=arr[0])
			res.push_back(arr[i]);
	}
	return res;
}

//O(n^2) time and O(n^2) space
bool sameBsts(vector<int> arrayOne, vector<int> arrayTwo) {
  // Write your code here.
	if(arrayOne.size()!=arrayTwo.size())
		return false;
	if(arrayOne.size()==0 && arrayTwo.size()==0)
		return true;
	if(arrayOne[0]!=arrayTwo[0])
		return false;
	vector<int> leftOne=getSmaller(arrayOne);
	vector<int> leftTwo=getSmaller(arrayTwo);
	vector<int> rightOne=getBiggerOrEqual(arrayOne);
	vector<int> rightTwo=getBiggerOrEqual(arrayTwo);
	return sameBsts(leftOne,leftTwo) && sameBsts(rightOne, rightTwo);
}

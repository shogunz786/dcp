#include <vector>
using namespace std;
//O(n) time and O(1) space
int kadanesAlgorithm(vector<int> arr) {
	int mxsofar = arr[0];
	int mxending = arr[0];
	for(int i=1; i<arr.size(); i++){
		int num = arr[i];
		mxending = max(num, mxending+num);
		mxsofar=max(mxsofar,mxending);
	}
	return mxsofar;
}

/*
  given array: [3, 5, -9, 1, 3, -2, 3, 4, 7, 2, -9, 6, 3, 1, -5, 4]
  Find max sum that can be obtained by summing up all
  of integers in non empty subarray of the given array.
  result: 19 - [ 1, 3, -2, 3, 4, 7, 2, -9, 6, 3, 1]
 */
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


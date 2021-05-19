/*
 Given a sorted array of ints
 return sorted array of squares of ints.
 input:[-5, -4, -3, -2, -1]
 output:[1, 4, 9, 16, 25]
 */
#include <vector>
using namespace std;

//O(nlogn) time and O(n) space
vector<int> sortedSquaredArray1(vector<int> array) {
	vector<int> res;
	for(auto a:array){
	  res.push_back(a*a);	
	}
	sort(res.begin(),res.end());
  return res;
}

//O(n) time and O(n) space
vector<int> sortedSquaredArray(vector<int> array) {
	vector<int> res(array.size(),0);
	int l=0, r=array.size()-1;
	for(int i=array.size()-1; i>=0; i--){
		if(abs(array[l])>abs(array[r])){
			res[i]=array[l]*array[l];
			l++;
		}else{
			res[i]=array[r]*array[r];
		  r--;	
		}
	}
	return res;
}

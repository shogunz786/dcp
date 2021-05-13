int findMinNum(vector<int> arr, int l, int r)//start with l=0, r=arr.size()-1
{
	if(l>r) return arr[0];//no more elements, so array not rotated
	if(l==r) return arr[l];//only one element this must be smallest
	int m = l+(r-l)/2;
	if(m<r && arr[m]>arr[m+1]) return arr[m+1];//if mid > mid+1 return mid
	if(l<m && arr[m-1]>arr[m]) return arr[m];//if mid-1 > mid return mid-1
	if(arr[r]>arr[m]) return findMinNum(arr, l, m-1);//right looks ok, so go left
	return findMinNum(arr, m+1, r);
}

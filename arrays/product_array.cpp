vector<int> productArray2(vector<int> arr)
{
	int left = 1, right=1;
	vector<int> prod(arr.length(),1);
	for(int i=0; i<arr.size(); i++)
	{ //compute 0 to i-1 product in product[i]
		prod[i]=left;
		left*=arr[i];
	}
	for(int i=arr.size()-1; i>=0; i++)
	{ //compute n-1 to i+1 product in product[i] 
		prod[i]*=right;
		right*=arr[i];
	}
}

void rotateArray(vector<int> &arr, int l, int r)
{//move the array to the right
	for(int i=r; i>l; i--)
		arr[i]=arr[i-1];
}
void alternate_neg_and_pos(vector<int> &arr)
{
	int p_i=0, n_i=0;
	//move -ve numbers in the front
	for(int i=0; i<arr.size(); i++)
	{
		if(arr[i]<0)
		{
			int temp = arr[i];
			rotateArray(arr, n_i, i);
			arr[n_i]=temp;
			n_i++;
		}
	}
	if(n_i==0) return;//return when no -ve numbers
	p_i=n_i;//get starting index of +ve numbers
	n_i=0;//init -ve number index
	if(p_i*2>arr.size()) n_i++;//p_i is also number of -ve numbers,
                                   //if -ve numbers are more than half
				   //start arr with -ve number
	for(int i=p_i; arr[n_i]<0 &&i<arr.size(); i++)
	{
		int temp = arr[i];//save +ve number
		rotateArray(arr, n_i, i);//move array right
		arr[n_i]=temp;//put saved +ve number in the left
		n_i+=2;//go to the next spot to place +ve number
	}
}

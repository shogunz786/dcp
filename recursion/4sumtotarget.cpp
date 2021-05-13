bool findTarget(vector<int> input, int k, int index, int sum, int count)
{
	if(index >= input.size())
		return false;
	if(count == 4)
	{
		if(sum == k)//target
			return true;
		return false;
	}
	//number is used for sum
	if(findTarget(input,k, i+1, sum+input[i], count+1))
		return true;
	//number is not used for sum
	if(findTarget(input,k, i+1, sum, count)) 
		return true;
	return false;
}

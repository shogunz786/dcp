void find_real_primes(vector<bool> &primes, int max)
{
	long long i=0, maxval=(long long)max;
	for(;i*i<=maxval;i++)
	{       //mark multiples of i*i as not primes
		for(long long j=i*i; j<=maxval; j+=i)
			primes[j]=false;
	}
}
string detect_primes(vector<int> arr)
{
	string str(arr.length(),1);//init all primes
	int max = *max_element(arr.begin(), arr.end());
	vector<bool> primes(max+1, true);
	primes[1]=false;//1 is not prime
	find_real_primes(primes, max);
	for(int i=0; i<arr.length(); i++)
	{
		if(!primes[arr[i]])
			str[i]='0';//undo not real primes
        }
	return str;
}

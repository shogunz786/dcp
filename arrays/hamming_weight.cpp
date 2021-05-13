int bitCount(int n)
{
	int count=0;
	while(n)
	{
		if(n&1) count++;
		n>>=1;
	}
	return count;
}
int printBitCount(vector<int> arr)
{
	int count=0;
	map<int,int> mp;
	//find the bit count till 0 to 0xff
	for(int i=0;i<256;i++)
		mp.insert(make_pair(i,bitCount(i)));
	for(int i=0;i<arr.size(); i++)
	{
		count+=mp[(arr[i]&0xff)]+//find bit count in first byte
			mp[((arr[i]>>8)&0xff)]+//find bit count in second byte
			mp[((arr[i]>>16)&0xf)]+//find bit count in third byte
			mp[((arr[i]>>24)&0xff)];//find bit count in fourth byte
	}
	return count;
}

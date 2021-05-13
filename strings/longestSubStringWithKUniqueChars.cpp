int lengthOfLongestSubStringWithKDistinctChars(string text, int k)
{
	if(k==0 || s.length()==0) return 0;
	vector<int> ascii(256,0);
	int len = 0;
	for(int i=0, start=0, count=0; i< text.length(); i++)
	{
		if(count<k)
		{//found char for first time
			if(ascii[text[i]]==0) count++
		}else if(ascii[text[i]]==0){//found new char
                   while(start<i){//iterate to decr
			   char c = text[start++];
			   ascii[c]--;
			   if(ascii[c]==0) break
		   }
		}
		ascii[text[i]]++;
		len = max(len, i-start+1);
	}
	return (count==k)?len:0;
}

string runlenCompress(string word)
{
	stringstream output;
	int count=0;
	for(int i=0; i<word.length()-1;i++)
	{

		if(word[i]==word[i+1])
			count++;//keep count while cur and next match
		else{
			if(count>1)//print count when >1
				output<<count;
			output<<word[i];
			if(i+1==word.length()-1)//if this is the last one print
				output<<word[i+1];
			count=1;

		}
	}
	return output.str();
}

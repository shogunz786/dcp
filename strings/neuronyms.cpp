void neuronyms(string text)
{
	for(int j=2; j<=text.length()-2; j++)
	{
		for(int i=0; (i+j+1)<text.length(); i++)
		{
			cout<<text.substr(0,i+1)<<j<<text.substr(i+j+1)<<endl;
		}
	}
}

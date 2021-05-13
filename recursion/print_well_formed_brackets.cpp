void generateBrackets(int size)
{
  for(int i=0; i<size; i++)
  {
     prepBrackets(string(""), 0, 0, i);
  }
}
void prepBrackets(string str, int open, int close, int pairs)
{
	if(open == pairs && close ==pairs)
	{
		cout<<str<<endl;
	}else{
		if(open<pairs){
			prepBrackets(str+'(', open+1, close, pairs);
		}else if(close<pairs){
			prepBrackets(str+')', open, close+1, pairs);
		}
	}
}


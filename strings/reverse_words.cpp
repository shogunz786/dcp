void reverseStr(string &str, int l, int r)
{
	while(l<r)
		swap(str[l++],str[r--]);
}
void reverse_words(string& text)
{ 
	int begin = 0, end=0, i=0;
	//reverse whole string
	reverseStr(text, 0, text.length()-1);
	while(i<text.length())
	{
		begin=i;//find each word
		while(i<text.length() && text[i]!=' ')
			i++;
		end=i;
		//reverse each word
		reverse(text, begin, end-1);
		i++;
	}
}

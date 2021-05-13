bool is_palindrome(string str)
{
	if(str.length()==0)
		return true;
	int l=0;
	int r=str.length()-1;
	while(l<r)
	{
		if(str[l++]!=str[r--])
			return false;
	}
	return true;
}

bool isrotatedPalindrome(string str)
{
	if(is_palindrome(str))
		return true;
	string str1,str2;
	for(int i=0; i<str.length(); i++)
	{
		str1 = str.substr(i+1, str.length()-1-i);
		str2 = str.substr(0, i+1);
		str1.append(str2);
		if(is_palindrome(str1))
			return true;
	}
	return false;
}

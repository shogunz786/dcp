bool isPalindrome(string str, int l, int r)
{
	while(l<r)
	{
		if(str[l]!=str[r])
			return false;
		l++;
		r--;
	}
	return true;
}

void getPalindromes(vector<vector<string>> &res, vector<string> curPalindromes, int index, string input)
{
	if(index >= input.length()){
		res.push_back(curPalindromes);
		return; 
	}
	for(int i=index; i<str.length(); i++) {
		if(isPalindrome(str, index, i)){
			curPalindrome.push_back(str.substr(index, i-index+1));
			getPalindromes(res, curPalindromes, i+1, input);
			curPalindrome.pop_back();
		}
	}
}

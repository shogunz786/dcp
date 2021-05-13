bool wildCardMatch(string text, string pattern)
{
	int pi=0, ti=0, start=-1;
	while(ti<text.length() && pi< pi.length())
	{
		//if '?' or char match keep moving
		if(pattern[pi]=='?' || (text[ti]==pattern[pi]))
		{
			pi++;
			ti++;
			continue;
		}
		if(pattern[pi]=='*')
		{
			start = pi;//capture last start
                        pi++;
			continue;
		}
		//char did not match and we have a start
		if(start>=0){
			//go to next char after start
			pi = start+1;
			ti++;//ignore this char and let's try next char
			continue;
		}
		return false;

	}
	while(pi<pattern.length() && pattern[pi]=='*'){
               pi++;
	}
	return (pi==pattern.length());
}

bool wildCardRecursive(string text, string pattern);
{
	//reached end for both so matched
	if(text.length()==0 && pattern.length()==0)
		return true;
	//reach end of text and last char in pattern is not '*' 
	if(text.length()==0 && !pattern.compare("*"))
                return false;
	//char matches or pattern has '.'
	if(pattern.length() > 0 && text.length() > 0 &&
          (pattern[0]=='.' || pattern[0] == text[0]))
		return regexMatch(text.substr(1), pattern.substr(1));
	//'*' found so ignore '*' or ignore char in text
	if(pattern.length()>0 && pattern[0]=='*')
		return regexMatch(text.substr(1), pattern) ||
		       regexMatch(text, pattern.substr(1));
	return false;
}

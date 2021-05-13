bool regexMatch(string text, string pattern);
{
	//reached end for both so matched
	if(text.length()==0 && pattern.length()==0)
		return true;
	bool first_match = (text.length() > 0 &&
          (pattern[0]=='.' || pattern[0] == text[0]));
	//'*' found so ignore '*' or ignore char in text
	if(pattern.length()>=2 && pattern[1]=='*')//next char is star
		return (regexMatch(text, pattern.substr(2)) ||//ignore star
	               //try if next char matches first match
		       (first_match && regexMatch(text.substr(1), pattern))));
	else
		return first_match && regexMatch(text.substr(1), pattern.substr(1));
}

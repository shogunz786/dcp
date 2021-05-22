/*
 "fileName": "abcdefg",
  "pattern": "a*e?g"
	'.' or '?' - don't care
	* - matches 0 or more or any number of chars
	return match or not result is true
*/
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
	//reach end of match time to make a decision
	if(pattern.length()==0){
	  if(text.length()==0){
		  //reached end for both so matched
		  return true;
		}else{
		  //did not reach end of text
		  return false;
		}
	}

	//reach end of text but may not be end of pattern
	if(text.length()==0){
	   if(!pattern.compare("*")){
	      //reach end of text and last char in pattern is not '*' 
              return false;
	   }
           //there could be more than one '*' let's check
	}

	//char matches or pattern has '.'
	if(pattern.length() > 0 && text.length() > 0 &&
          (pattern[0]=='.' || pattern[0] == text[0])){
		return wildCardRecursive(text.substr(1), pattern.substr(1));
	//'*' found so ignore '*' or ignore char in text
	}else if(pattern.length()>0 && pattern[0]=='*')
		//could have reached end of text but did not reach end of the pattern
		return (text.length()>0 && wildCardRecursive(text.substr(1), pattern)) ||
		       wildCardRecursive(text, pattern.substr(1));
	return false;
}

//O(n*m) space and O(n*m) time
bool globMatching(string filename, string pattern){
	//init dp with filename.length()+1 X patten.length()+1 matrix inited to false
	vector<vector<bool>> dp(filename.length()+1, vector<bool>(pattern.length()+1, false));
	
	//when filename and pattern lengths are zero match is true
  dp[0][0]=true;	
	
	for(int j=1; j<pattern.length()+1; j++){
		if(pattern[j-1]=='*'){
			  //if cur pattern '*' get match status from previous column
				dp[0][j]=dp[0][j-1];
		}
	}
	
	for(int i=1; i<filename.length()+1; i++){
		for(int j=1; j<pattern.length()+1; j++){
			//if char match or don't care , match is same as previous match
			if(filename[i-1]==pattern[j-1] || pattern[j-1]=='?'){
			   dp[i][j]=dp[i-1][j-1];	
			//if cur pattern is '*' get the true match from previous match with last pattern match or
			//last filename match
			}else if(pattern[j-1]=='*'){
				 dp[i][j]=dp[i-1][j] || dp[i][j-1];
			}
		}
	}
	return dp[filename.length()][pattern.length()];
}


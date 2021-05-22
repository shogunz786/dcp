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



using namespace std;
bool helper(string filename, string pattern, int fIdx=0, int pIdx=0, string tab=""){
	cout<<tab<<__FILE__<<":"<<__LINE__<<" fIdx="<<fIdx<<", pIdx="<<pIdx<<endl;
	//scanned all the pattern so match is good
	if(pattern.length()==pIdx && filename.length()==fIdx){
	cout<<tab<<__FILE__<<":"<<__LINE__<<" fIdx="<<fIdx<<", pIdx="<<pIdx<<endl;
	   return true;
	}
	
	if(filename.length()==fIdx){
		if(pattern[pIdx]!='*'){
	cout<<tab<<__FILE__<<":"<<__LINE__<<" fIdx="<<fIdx<<", pIdx="<<pIdx<<endl;
			return false;
		}else if(pattern[pIdx]!='*' &&
						 pIdx+1==pattern.length()){
	cout<<tab<<__FILE__<<":"<<__LINE__<<" fIdx="<<fIdx<<", pIdx="<<pIdx<<endl;
			return true;
		}
	}
	
	//if don't care or char match skip and go to next char in filename and pattern
	if(pattern[pIdx]=='?' || filename[fIdx]==pattern[pIdx]){
	cout<<tab<<__FILE__<<":"<<__LINE__<<" fIdx="<<fIdx<<", pIdx="<<pIdx<<endl;
		return helper(filename, pattern,fIdx+1,pIdx+1,tab+" ");
	}else if(pattern.length()>0 && 
					 pattern[pIdx]=='*'){
	cout<<tab<<__FILE__<<":"<<__LINE__<<" fIdx="<<fIdx<<", pIdx="<<pIdx<<endl;
		//check scenario where ignore filename char but continue to match '*'
		//match '*' with nothing and continue with next pattern index
		return helper(filename, pattern,pIdx+1,fIdx,tab+" " ) || 
			     helper(filename, pattern,pIdx,fIdx+1,tab+" " );
	}
	
	cout<<tab<<__FILE__<<":"<<__LINE__<<" fIdx="<<fIdx<<", pIdx="<<pIdx<<endl;
	return false;
	 
}
bool globMatching1(string fileName, string pattern) {
  return helper(fileName, pattern);
}

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


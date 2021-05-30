 
/*
Input: words = ["a","b","ba","bca","bda","bdca"]
Output: 4
Explanation: One of the longest word chain is "a","ba","bda","bdca".

A word chain is a sequence of words [word_1, word_2, ..., word_k]
with k >= 1, where word_1 is a predecessor of word_2, word_2 
is a predecessor of word_3, and so on.

Return the longest possible length of a word chain 
with words chosen from the given list of words.
*/
bool cmpString(string a, string b){
      return a.length()<b.length();
 }
//O(n*longest string length) time and O(n) space
 int longestStrChainLength(vector<string>& words) {
     //sort strings to complete the chains of smallest strings first
     sort(words.begin(), words.end(), cmpString);
     unordered_map<string,int> dp;
     int maxSize=0;
     for(auto w: words){
         //for each word look at different substrings
         for(int i=0; i<w.length(); i++){
             //make different substrings
             string str = w.substr(0,i)+w.substr(i+1);
             //check if new substring is present in dictionary, if yes add 1 to current chain length
             //else set 1
             dp[w]=max(dp[w],dp.find(str)==dp.end()?1:dp[str]+1);
         }
         //find the longest so far
         maxSize=max(maxSize, dp[w]);
     }
     return maxSize;
}


//O(n*longest string length) time and O(n) space
vector<string> longestStringChainPath(vector<string> strings) {
	//dp to track longest chain
	unordered_map<string,int> dp;
  //dp to track path to longest chain	
	unordered_map<string,string> chain;
	//capture results
	vector<string> result;
	//capture start string of longest chain
	string longChainStr;
	//one word is the base case of longest chain
	int longChainLen=1;
	
	//sort to process smallest strings first
	sort(strings.begin(),strings.end(),cmpString);
	
	for(auto str : strings){
		//init chain for each word ending with empty string
		chain[str]="";
		for(int i=0; i<str.length(); i++){
			//generate substrings
			string subStr = str.substr(0,i)+str.substr(i+1);
			//if no path found then set length to 1
			if(dp.find(subStr)==dp.end()){
				dp[str]=max(dp[str],1);
			}else{
				//found a path greater than current path
				//increment the new path with 1
				if(dp[subStr]+1>dp[str]){
					dp[str]=dp[subStr]+1;
				  //capture the sub string providing
				  //the longest path
					chain[str]=subStr;
				}
			}
		}
		//keep track of the longest path and the starting
		//string of the longest path
		if(longChainLen<dp[str]){
			longChainLen=dp[str];
			longChainStr=str;
		}
	}
	
  while(chain.find(longChainStr)!=chain.end()){
		   //capture path if its valid
		   if(longChainStr!="")
	        result.push_back(longChainStr);
		   longChainStr=chain[longChainStr];
 }
	
  return result;
}

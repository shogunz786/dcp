 
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
 int longestStrChain(vector<string>& words) {
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

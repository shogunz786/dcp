 bool cmpString(string a, string b){
      return a.length()<b.length();
 }

 int longestStrChain(vector<string>& words) {
     sort(words.begin(), words.end(), cmpString);
     unordered_map<string,int> dp;
     int maxSize=0;
     for(auto w: words){
         for(int i=0; i<w.length(); i++){
             string str = w.substr(0,i)+w.substr(i+1);
             dp[w]=max(dp[w],dp.find(str)==dp.end()?1:dp[str]+1);
         }
         maxSize=max(maxSize, dp[w]);
     }
     return maxSize;
}

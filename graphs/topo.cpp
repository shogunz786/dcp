void topo(vector<string> &words)
{
  unordered_map<char, unordered_set<char>> adj;
  unordered_map<char, int> degree;
  for(string word: words)
  {
    for(char c: word)
    {
       degree[c]=0;
    }
  }
  for(int i=0; i<words.size()-1; i++)
  {
    int minz = min(words[i].length(), words[i+1].length());
    for(int j=0; j<minz; j++)
    {
      if(words[i][j]!=words[i+1][j])
      {
         unordered_set<char> su = adj[words[i][j]];
         if(su.find(words[i][j]) == su.end())
         {
           adj[words[i][j].insert(words[i+1][j]);
           degree[words[i+1][j]]++;
         }
         break;
      }
    }
  }
 queue<char> sc;
 for(auto& e: degree){
   if(e.second==0)
    sc.push(e.first);
 }
 string res;
 while(!sc.empty()){
  char c = sc.front();
  sc.pop();
  res+=c;
  if(adj[c].size()){
   for(auto& e: adj[c]){
     degree[e]--;
     if(degree[e]==0)
       sc.push(e);
   }
  }
 }

 return res.length() == degree.size() ? res: "";
  
}

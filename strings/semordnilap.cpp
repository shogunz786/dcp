using namespace std;

//O(n*m) time and O(n*m) space - n number of words, m length of longest word
vector<vector<string>> semordnilap(vector<string> words) {
  unordered_set<string> st(words.begin(), words.end());
  vector<vector<string>> res;
  for(auto w: words){
    string r = w;
    reverse(r.begin(), r.end());
    if(st.find(r)!=st.end() && r!=w){
        st.erase(r);
        st.erase(w);
        res.push_back({w,r});
    }
  }
  return res;
}

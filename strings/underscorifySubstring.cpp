using namespace std;

//merge intervals
//Average time O(n+m) and space O(n)

//input str : testhello world but testtest is repeating testest
//      substr: test
//output : _test_hello world but _testtest_ is repeating _testest_
vector<vector<int>> prepareIntervals(string &str, string &subStr){
  int idx=0;
  int end = 0;
  vector<vector<int>> res;
  while(idx<str.length()){
    idx = str.find(subStr, idx);
    //if no substr break
    if(idx == string::npos){
      break;
    }

    end = idx+static_cast<int>(subStr.length());
    //update the end of the substr if overlap or adjacent
    if(res.size() >0 && res.back()[1]>=idx){
      //merge with previous
      res.back()[1]=end;
    }else{
      //create new entry when no overlap
      res.push_back(vector<int>{idx, end});
    }
    //try from next idx
    idx++;
  }
  return res;
}

string underscorifySubstring(string str, string subStr) {
  int idx = 0;
  int len = 0;
  vector<string> strs;
  vector<vector<int>> intervals = prepareIntervals(str, subStr);
  //iterate till end of the vector
  for(auto v: intervals){
    //first add prefix when no immediate match
    if(v[0]!=idx){
      //len of prefix
      len = v[0]-idx;
      strs.push_back(str.substr(idx, len));
    }
    strs.push_back("_");
    //find len of repeating substr
    len = v[1]-v[0];
    //append repeating substr - testtest or testest
    strs.push_back(str.substr(v[0], len));
    
    strs.push_back("_");
    //keep track of previous end
    idx=v[1];
  }
  //add remaining part of the string
  if(idx<str.length()){
    strs.push_back(str.substr(idx));
  }
  return accumulate(strs.begin(), strs.end(), string());
}

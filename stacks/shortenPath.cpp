/*
 * shorten the relative path
 * e.g:
       in:"/foo/../test/../test/../foo//bar/./baz"
       out:"/foo/bar/baz"
       in:"foo/bar/baz"
       out:"foo/bar/baz"
       in:"/../../foo/bar/baz"
       out:"/foo/bar/baz"
 */
       

#include <sstream>
#include <vector>
#include <iostream>
using namespace std;


// /foo
// foo
// ./foo
// ../foo
// foo/..

string shortenPath(string path) {
  istringstream sstr(path);
  string word;
  vector<string> tokens;
  vector<string> st;

  //parse tokens, when starting with '/' first token is empty string
  while(getline(sstr, word, '/')){
    tokens.push_back(word);
  }
  //filter tokens	
  for(int i=0; i<tokens.size(); i++){
      string t = tokens[i];
      if(i==0 && t == ""){//keep first empty to record starting '/'
        st.push_back(t); 
      }else if(t=="." || t==""){//remove all other '.' and ''
        continue;
      }else if(t==".."){
        if(st.size()==0 || st.back()==".."){//st empty means not obsolute path and last is '..' then keep the new '..'
            st.push_back(t); 
        }else if(st.back()!=""){//keep begining empty string for obsolute path
          st.pop_back();
        }
      }else{
        st.push_back(t);  
      }
  }
  ostringstream res;

  for(int i=0; i<st.size(); i++){
      if(st[i]==""){//first empty token means starts with '/'
        res<<"/";
      }else{
        if(i!=0 && res.str()[res.str().length()-1]!='/'){//if obsolute path no need to add one more '/'
          res<<'/'; 
        }
        res<<st[i];
      }
  }
  return res.str();
}

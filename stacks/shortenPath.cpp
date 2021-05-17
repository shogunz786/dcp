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
       

#include <string>
#include <sstream>
#include <stack>
#include <iostream>
using namespace std;

bool isEmpty(string &str){
	for(auto s: str){
		if(s != ' '){
			return false;
		}
	}
	return true;
}

//O(n) time and O(n) space
string shortenPath(string path) {
  // Write your code here.
	stringstream ss(path);
	int i=0;
	string str;
	vector<string> vstr;
	vector<string> st;
	
	//get all tokens except // and /./ patterns
	//ignore them
	while(getline(ss,str,'/')){
		if(str!="." && str.length()){
		   vstr.push_back(str);	
		}
	}
	
	//add placeholder if path begins with /
	if(path[0]=='/'){
		st.push_back("");
	}
  for(auto str: vstr){	
		if(str==".."){
		  if(st.size()==0 || st[st.size()-1]==".."){
				//first token is .. or previous is also ..
				st.push_back(str);
			}else if(st[st.size()-1] != ""){
				//pop previous dir
				st.pop_back();
			}	
		}else{
			//add to list
			st.push_back(str);
		}
	}
	if(st.size()==1 && st[0]==""){
		//only one entry with / marker
		return "/";
	}
  ostringstream oss;	
	for(auto i=0; i<st.size(); i++){
		if(i!=0)
			//do not add / for first entry
			oss<<"/";
		oss<<st[i];
	}
  return oss.str();
}

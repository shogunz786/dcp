/*
 Given number of tags, return all combinations of valid
 open and close tags with total tags is equal to given number of tags.
 tags:3
 result:
 ["<div><div><div></div></div></div>",
 "<div><div></div><div></div></div>", 
 "<div><div></div></div><div></div>",
 "<div></div><div><div></div></div>",
 "<div></div><div></div><div></div>"]
 */
#include <vector>
using namespace std;
//O((2n)!)/((n!(n+1)!)) time and O((2n)!)/((n!(n+1)!)) space
void helper(vector<string>& res, string str, int op, int cl,string tab){
 //if closing reached 0 capture the string
	if(cl==0){
		res.push_back(str);
		return;
	}
	//open is > 0 add open suffix
	if(op>0){
	  //create new string opbject to avoid carrying previous strings
		string s = str+"<div>";
		//reduce open by 1
	  helper(res, s, op-1, cl, tab+" ");
	}
	//opens are less than close
	if(op<cl){
	  //create new string opbject to avoid carrying previous strings
		string s = str+"</div>";
		//reduce close by 1
	  helper(res, s, op, cl-1, tab+" ");
	}
}

vector<string> generateDivTags(int numberOfTags) {
	vector<string> res;
	helper(res,"",numberOfTags,numberOfTags,"");
  return res;
}

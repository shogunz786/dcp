#include<vector>
using namespace std;

void getLocations(vector<vector<int>> &vv,string &str, string &subStr)
{
	int b=0;
	while(b<str.length()){
		if(str.length()-b < subStr.length()){
			break;
		}
		int f = str.find(subStr, b);
		if(f != string::npos){
		   vv.push_back({f, f+static_cast<int>(subStr.length())-1});
		   b=f+1;
		}else{
			break;
		}
	}
}

void collapse(vector<vector<int>> &vv, vector<vector<int>> &ret){
	if(vv.size()==0){
		return;
	}
	for(int i=1; i<vv.size(); i++){
		if(vv[i][0]<=vv[i-1][1] || vv[i][0]-1==vv[i-1][1]){
			vv[i][0]=vv[i-1][0];
		}else{
			ret.push_back(vv[i-1]);
		}
	}
	ret.push_back(vv[vv.size()-1]);
}

void buildString(string &str, vector<vector<int>> &ret){
	string res;
	int begin=0, end=0;
	int b=0;
	for(int i=0; i<ret.size(); i++){
		if(b!=ret[i][0]){
			res+=str.substr(b,ret[i][0]-b);
		}
		res+="_"+str.substr(ret[i][0],ret[i][1]-ret[i][0]+1)+"_";
		b=ret[i][1]+1;
	}
	if(b<str.length()){
			res+=str.substr(b,str.length()-b);
	}
	str=res;
}

//O(n+m) time and O(n) space
string underscorifySubstring(string str, string subStr) {
	if(str.length()<=subStr.length()){
		return str;
	}
  // Write your code here.
	vector<vector<int>> vv, ret;
	getLocations(vv, str, subStr);
	cout<<"Locations:"<<endl;
	for(auto a:vv){
		cout<<a[0]<<","<<a[1]<<endl;
	}
	collapse(vv, ret);
	cout<<"Collapse:"<<endl;
	for(auto a:ret){
		cout<<a[0]<<","<<a[1]<<endl;
	}
	buildString(str, ret);
  return str;
}

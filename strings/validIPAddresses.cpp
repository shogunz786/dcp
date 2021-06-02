#include <vector>
using namespace std;

bool isValid(string &str){
	if(str.length()==0) return false;
	if(str.length()>1 && str[0]=='0') return false;
	int i=atoi(str.c_str());
  if(i>=0 && i<256)	return true;
	return false;
}
//O(1) time and O(1) space
vector<string> validIPAddresses(string str) {
  // Write your code here.
	vector<string> v;
	for(int i=1; i<4 && i<str.length(); i++){
		 string one = str.substr(0,i);
		 if(!isValid(one)) continue;
		
		 for(int j=1; j<4 && (i+j)<str.length(); j++){
			 string two=str.substr(i,j);
			 if(!isValid(two)) continue;
			 
			 for(int k=1; k<4 && (i+j+k)<str.length(); k++){
				   string three=str.substr(i+j,k);
           if(!isValid(three)) continue;
					
					 string four=str.substr(i+j+k);
					 if(!isValid(four)) continue;
					
					 string str_out;
					 str_out=one+"."+two+"."+three+"."+four;
					 cout<<str_out<<endl;
					 v.push_back(str_out);
				}
		 }
	}
	return v;
}

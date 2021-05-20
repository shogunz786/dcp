/*
 given a string with space seperated words
 return string with reversed words with same spacing
 input: "this      string     has a     lot of   whitespace"
 result:"whitespace   of lot     a has     string      this"
 */
using namespace std;
//O(n) time and O(n) space
string reverseWordsInString(string str) {
	vector<string> v;
	int l=0;
	int i=0;
	//go thru each character of string
	while(i<str.length()){
		//capture the contigous spaces
		if(str[i]==' '){
			l = i;
			while(i<str.length() && str[i]==' '){
				i++;
			}
			v.push_back(str.substr(l,i-l));
		}else{
			//capture the contingous characters
			l=i;
			while(i<str.length() && str[i]!=' '){
				i++;
			}
			v.push_back(str.substr(l,i-l));
		}
	}
	//prepare result string by reverse appending the words and spaces
	//captured before
	string res;
	for(int i=v.size()-1; i>=0; i--){
		res+=v[i];
	}
  return res;
}


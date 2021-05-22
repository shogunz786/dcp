/*
  input strings: ["foobarbaz", "foo", "bar", "foobarfoo", "baz", "foobaz", "foofoofoo", "foobazar"]
	output strings: ["foobarbaz", "foobarfoo", "foobaz", "foofoofoo"]
  
	Find all strings that could be formed with other strings in the strings array
	*/
using namespace std;

//recursively check from the last match index if there are other matches
bool findMatch(vector<string> &strings, string str, int strIdx){
	   if(str.length()==0){
			 return true;
		 }
	
     for(int i=0; i<strings.size(); i++){
			 //do not match with same string
			 if(i==strIdx){
				 continue;
			 }
			 //if the match starts from beginning of the string
			 if(str.find(strings[i])==0){
				  //progress the string to the new starting index of the match
				  //continue to test with other strings till there is a match
				  if(findMatch(strings, str.substr(strings[i].length()), strIdx)){
						//on match return true
						return true;
					}
			 }
		 }	
	   //no match so far return false
	   return false;
}

vector<string> specialStrings(vector<string> strings) {
	vector<string> vstr;
	//check if each string in the array matches one of the other strings
	for(int i=0; i<strings.size(); i++){
		if(findMatch(strings, strings[i], i)){
		  vstr.push_back(strings[i]);	
		}
	}
  return vstr;
}

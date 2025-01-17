#include <vector>
using namespace std;

//numTags = 3
//result - ["<div><div><div></div></div></div>",
//          "<div><div></div><div></div></div>",
//          "<div><div></div></div><div></div>",
//          "<div></div><div><div></div></div>",
//          "<div></div><div></div><div></div>"]

//O((2n)!/((n!((n+1)!)))) time and O((2n)!/((n!((n+1)!)))) space
void generateDivTagsHelper(int openingTag, int closingTag, string prefix, vector<string> &result){

  if(openingTag > 0){
    //add opening tag
    generateDivTagsHelper(openingTag-1, closingTag, prefix+"<div>", result);
  }

  if(openingTag<closingTag){
    //add closing tag
    generateDivTagsHelper(openingTag, closingTag-1, prefix+"</div>", result);
  }
  if(closingTag==0){
    //collect result
    result.push_back(prefix);
  }
  
}

vector<string> generateDivTags(int numberOfTags) {
  vector<string> result;
  generateDivTagsHelper(numberOfTags, numberOfTags, "", result);
  return result;
}
//time and complexity O(2^2n*n)
    vector<string> generateDivTags(int n) {
        vector<string> v;
        queue<string> q;
        q.push("<div>");
        while(!q.empty()){
            string str = q.front();
            q.pop();
            if(str.length() == 2*n){
                if(isValid(str)){
                    v.push_back(str);
                }
                continue; 
            }
            q.push(str+"<div>");
            q.push(str+"</div>");
        }
        return v;
    }

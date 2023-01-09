#include <vector>
using namespace std;

void phoneNumberMnemonicsHelper(int i,unordered_map<char,vector<char>>& phone,
string &phoneNumber, string mnemnonic, vector<string> &res){
  if(i==phoneNumber.size()){
    res.push_back(mnemnonic);
  }else{
    char ch = phoneNumber[i];
    vector<char> chars = phone[ch];
    for(auto c: chars){
      mnemnonic[i]=c;
      phoneNumberMnemonicsHelper(i+1, phone, phoneNumber, mnemnonic, res);
    }
  }
}
vector<string> phoneNumberMnemonics(string phoneNumber) {
  unordered_map<char,vector<char>> phone =
  {{'1',{'1'}},
  {'2',{'a','b','c'}},
  {'3',{'d','e','f'}},
  {'4',{'g','h','i'}},
  {'5',{'j','k','l'}},
  {'6',{'m','n','o'}},
  {'7',{'p','q','r','s'}},
  {'8',{'t','u','v'}},
  {'9',{'w','x','y','z'}},
  {'0',{'0'}}};

  string mnemnonic(phoneNumber.size(), '0');
  vector<string> res;

  phoneNumberMnemonicsHelper(0, phone, phoneNumber, mnemnonic, res);
  return res;
}

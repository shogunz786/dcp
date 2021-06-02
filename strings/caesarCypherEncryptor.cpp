using namespace std;

//O(n) time and O(n) space
string caesarCypherEncryptor(string str, int key) {
	key=key%26;
	for(int i=0; i<str.length(); i++){
		 int idx=(str[i]-'a'+key)%26;
		 str[i]='a'+idx;
	}
	return str;
}

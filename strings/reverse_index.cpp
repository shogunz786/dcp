struct Trie{
	map<char,struct Trie*> child;
	bool end;
	vector<int> pos;
	Trie(){end=false;}
};

void insert(struct Trie* root, string word, int loc){
	if(!root || word.length()==0) return;
	for(int i=0; i<word.length(); i++)
	{
		if(root->child.find(word[i])==root->child.end())
			root->child.insert(make_pair(word[i],new Trie()));
		root = root->child[word[i]];
	}
	root->end = true;
	root->pos.push_back(loc);
}
//replace non-alpha numeric with spaces and convert all to lower
void cleanup(string &line)
{
        for(int i=0;i<line.length();i++)
        {
                if(line[i]>='a' && line[i]<='z')
                        continue;
                if(line[i]>='A' && line[i]<='Z')
                        line[i]=tolower(line[i]);
                else
                        line[i]=' ';
        }
}

void loadLine(string line, int start, struct Trie* root)
{
	int i=0;
	size_t space = 0;
	cleanup(line);
	while(i<line.length())
	{
		space = line.find(' ', i);
		if(space != string::npos)
		{
			if(space!=i)//ignore if threre are double spaces
				insert(root, line.substr(i, space-i),start+i+1);
			i=space+1;
		}else{
			insert(root, line.substr(i),start+i+1);
			break;//reached end
		}
	}
}

vector<int> findPos(struct Trie* root, string word)
{
	if(!root) return vector<int>(0);
	transform(word.begin.begin,word.end(),word.end(),tolower);
	for(int i=0; i<word.length() && root; i++)
	{
		root = root->child[word[i]];
	}
	return root?root->pos:vector<int>(0);
}
void dumpTrie(struct Trie* root, string tab)
{
  map<char, struct Trie*>::iterator iter = root->child.begin();
  for(;iter!=root->child.end(); iter++)
  {
     cout<<tab<<iter->first;
     if(iter->second->end)
     {
      cout<<"[";
      for(int i=0; i<iter->second->pos.size(); i++)
      {
        cout<<iter->second->pos[i]<<",";
      }
      cout<<"]"<<endl;
     }
     cout<<endl;
     dumpTrie(iter->second, tab+"  ");
  }
}


void buidTrie(string filename, struct Trie* root)
{
	ifstream ifs(filename.c_str());
	string line;
	int start = 0;
	while(getline(ifs,line)){
		loadLine(line, start, root);
		start+=line.length();
	}
}

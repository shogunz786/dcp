struct Trie{
	char c;
        map<char,struct Trie*> child;
	bool isend;
	Trie(){ isend=false; }
};

struct validStart
{
	int r,c;
	string s;
	struct Trie* t;
	validStart(int i, int j, struct Trie* tr, string str)
	{
		r=i;
		c=j;
		t=tr;
		s=str;
	}
};

void insert(struct Trie *root, string word)
{
	if(!root) return;
	for(int i=0;i<word.length(); i++)
	{
		if(root->child.find(word[i])==root->child.end())
			root->child.insert(make_pair(word[i],new Trie()));
		root = root->child[word[i]];
	}
	root->isend=true;
}
//tab for formating
void printTrie(struct Trie* root, string tab)
{
	map<char, struct Trie*>::iterator iter=root->child.begin();
	for(;iter!=root->child.end();iter++)
	{
		cout<<tab<<iter->first<<endl;
		printTrie(iter->second, tab+" ")
	}
}

void buildTrie(vector<string> words, struct Trie* root)
{
	//build dictionary
	for(int i=0; i<words.length(); i++)
		insert(root, words[i]);
}


//identify all chars which are starting chars of words in dictionary, to filter out unnecessary paths
queue<struct validStart*> buildQueueOfValidStartingChars(struct Trie *root, vector<vector<char>> boggle)
{
	queue<struct validStart *> validStarts;
	if(!root) return validStarts;
	for(int i=0; i<boggle.size(); i++)
	{
		for(int j=0;j<boggle[0].size(); j++)
		{
			if(root->child.find(boggle[i][j])!=root->child.end())
			{
				string str(1,boggle[i][j]);
				validStarts.push(new validStart(i,j,root->child[boggle[i][j]],str));
			}
		}
	}
	return validStarts;
}

vector<string> searchWords(queue<struct validStart*> validStarts, vector<vector<char>> boggle)
{
	vector<string> res;
	//scan all cells around
	int row[8]={-1, -1, -1, 0, 1, 1, 1, 0};
	int col[8]={-1, 0, 1, 1, 1, 0, -1, -1};
	while(!validStarts.empty())
	{
		struct validStart* valids = validStarts.front();
		validStarts.pop();
		for(int i=0;i<8;i++)
		{
			if(row[i]+valids->r>=0 && row[i]+valids->r<boggle.size() &&
					col[i]+valids->c>=0 && col[i]+valids->c<boggle[0].size())
			{
				char ch = boggle[row[i]+valids->r][col[i]+valids->c];
				if(valids->t->child.find(ch)!=valids->t->child.end())
				{
					valids->s += ch;
					if(!valids->t->child[ch]->isend)//at end this is the word
						res.push_back(valids->s);
					//update the validStart with new starting position and append the found char to string
					valids->r+=row[i];
					valids->c+=col[i];
					valids->t = valids->t->child[ch];
					validStarts.push(valids);
				}
			}
		}
	}
}

vector<string> findWords(vector<string> words, vector<vector<char>> boggle)
{
	struct Trie* root = new Trie();
	//build a trie from the word list
	buildTrie(words, root);
	//identify all valid start chars in boggle for possible match
        queue<struct validStart*> validStarts = buildQueueOfValidStartingChars(root, boggle);
	//go thru the trie to find words
        return searchWords(validStarts, boggle);
}

void inorder(Node *root, vector<int> &output)
{
	if(root==NULL)
		return;
	inorder(root->left, output);
	output.push_back(root->data);
	inorder(root->right, output);
}

vector<int> merge(vector<int> L, vector<int> R)
{
	if(L.size()==0)
		return R;
	if(R.size()==0)
		return L;
	int i,j;
	vector<int> res;
	i=0;
	j=0;	
	while(i<L.size() && j<R.size())
	{
		if(L[i]<R[j])
		{
			res.push_back(L[i++]); 
		}else{
			res.push_back(R[j++]); 
		}
	}

	while(i<L.size())
		res.push_back(L[i++]); 

	while(j<R.size())
		res.push_back(R[j++]); 

	return res;
}

Node* buildBST(vector<int> &input, int l, int r)
{
    if(l>r)
	return NULL;
    int m = l+(r-l)/2;
    Node *root = new Node(input[m]);
    root->left = buildBST(input, l, m-1);
    root->right = buildBST(input, m+1, r);
    return root;
}

Node* mergeTwoBST(Node *one, Node *two)
{
	vector<int> output1, output2, output3;
	inorder(one, output1);
	inorder(two, output2);
	output3 = merge(output1,output2);
	return buildBST(output3, 0, output3.size()-1);
}

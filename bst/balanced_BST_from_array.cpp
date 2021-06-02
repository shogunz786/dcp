Node * getBST(vector<int> input, int l, int r)
{
	if(l>r)
	   return NULL;
	int m = l+(r-l)/2;
	Node * root= new Node(input[m]);
	root->left = getBST(input, l, m-1);
	root->right = getBST(input, m+1, r);
	return root;
}

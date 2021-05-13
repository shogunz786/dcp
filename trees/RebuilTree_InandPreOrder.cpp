int findInIndex(vector<int> in, int root_value)
{
	for(int i=0; i<in.size(); i++)
		if(root_value==in[i]) return i;
	return -1;
}
Node *buildTree(vector<int> in, int in_l, int in_r, vector<int> pre,int pre_l, int pre_r)
{
	if(in_l>in_r || pre_l>pre_r) return NULL;
	int in_index = findInIndex( in, pre[pre_l]);//find root value index in inorder traversal
	Node * root = new Node(pre[pre_l]);//build node
	root->left = buildTree(in, in_l, in_index-1, pre, pre_l+1, pre_l+in_index-in_l);
	root->right = buildTree(in, in_index+1, in_r, pre, pre_l+in_index-in_l+1, pre_r);
	return root;
}

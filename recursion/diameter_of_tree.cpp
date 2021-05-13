int height(Node *root)
{
  if(root==NULL)
     return 0;
  return (1+max(height(root->left),height(root->right)));
}
int diameter(Node *root)
{
	if(root == NULL)
		return 0;
	return max(1+height(root->left)+
		  height(root->right)+
	  max(diameter(root->left),
	      diameter(root->right)));
}

//init min INT_MIN and max with INT_MAX
bool isBST(Node *root, int min, int max)
{
  if(root==NULL)
     return true;
  if(root->data < min || root->data > max)
     return false;
  return (isBST(root->left, min, root->data-1) && 
	  isBST(root->right, root->data+1, max));
}

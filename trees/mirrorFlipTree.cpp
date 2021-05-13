void flipTree(Node *root)
{
   if(!root) return NULL;
   Node * temp = root->left;
   root->left = root->right;
   root->right = temp;
   flipTree(root->right);
   flipTree(root->left);
}

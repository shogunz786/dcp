Node *cloneTree(Node *root)
{
  if(!root) return NULL;
  Node *clone = new Node(root->data);
  clone->left = cloneTree(root->left);
  clone->right = cloneTree(root->right);
  return clone;
}

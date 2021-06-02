void BSTtoDoubleLL(Node *root, Node **head)
{
   if(root ==NULL)
     return NULL;
   Node *prev = NULL;
   root->prev = BSTtoDoubleLL(root->left);
   if(prev){
      root->left = prev;
      prev->right = root;
   }else{
      *head = root;
   }
   prev = root;
   root->prev = BSTtoDoubleLL(root->right);
}

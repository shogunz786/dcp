void printAllPaths(Node *root, vector<int> path)
{
   if(root==NULL)
      return;
   //when reached a leaf get ready to print
   if(root->left == NULL && root->right == NULL)
   {
      path.push_back(root->data);
      printPath(path);
      return;
   }
   path.push_back(root->data);
   printAllPaths(root->left, path);
   printAllPaths(root->right, path);
}

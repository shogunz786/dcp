class BST {
public:
  int value;
  BST *left = nullptr;
  BST *right = nullptr;

  BST(int value) { this->value = value; }
};

//O(n^2) time and O(n) space
BST *reconstructBst(vector<int> preOrderTraversalValues) {
  if(preOrderTraversalValues.size()==0){
    return NULL;
  }
  int root = preOrderTraversalValues[0];

  int index = preOrderTraversalValues.size();
  for(int i=1; i<preOrderTraversalValues.size(); i++){
    if(preOrderTraversalValues[i]>=root){
      index = i;
      break;
    }
  }
  vector<int> left(preOrderTraversalValues.begin()+1,
                   preOrderTraversalValues.begin()+index);
  vector<int> right(preOrderTraversalValues.begin()+index,
                   preOrderTraversalValues.end());
  BST *b = new BST(root);
  b->left = reconstructBst(left);
  b->right = reconstructBst(right);
  return b;
}

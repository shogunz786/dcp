using namespace std;

/*
  Binary tree with atleast one node and check if it can be split into two binary trees of
  equal sum by removing a single edge. Sum of binary tree is sum of all values in the binary tree
*/
class BinaryTree {
public:
  int value;
  BinaryTree *left = nullptr;
  BinaryTree *right = nullptr;

  BinaryTree(int value) { this->value = value; }
};
struct TrackSum{
   int sum;
   bool canSplit;
};
int getTreeSum(BinaryTree *tree){
  if(tree==NULL){
    return 0;
  }
  return tree->value+getTreeSum(tree->left)+getTreeSum(tree->right);
}

TrackSum trySubTrees(BinaryTree *tree, int match){
   if(tree==NULL){
     return TrackSum{0, false};
   } 
   TrackSum left = trySubTrees(tree->left, match);
   TrackSum right = trySubTrees(tree->right, match);
   //keep track of sum
   int sum = tree->value+left.sum+right.sum;
   //check if match is found
   bool canSplit = sum == match || left.canSplit || right.canSplit;
   return TrackSum{sum, canSplit};
}

int splitBinaryTree(BinaryTree *tree) {
  //caliculate total sum of the BT
  int total = getTreeSum(tree);
  //if total odd can't split
  if(total%2){
    return 0;
  }
  int match = total/2;
  //check if subtrees have a matching half
  TrackSum res = trySubTrees(tree, match);
  return res.canSplit? match : 0;
}

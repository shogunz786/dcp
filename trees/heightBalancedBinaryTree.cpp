class BinaryTree {
public:
  int value;
  BinaryTree *left = nullptr;
  BinaryTree *right = nullptr;

  BinaryTree(int value) { this->value = value; }
};
class TreeInfo {
public:
bool balanced;
int height;
TreeInfo(bool isBalanced, int h){
  balanced = isBalanced;
  height = h;
}
};

TreeInfo helper(BinaryTree *tree){
  if(tree==NULL){
    return TreeInfo(true, -1);
  }
  auto leftInfo = helper(tree->left);
  auto rightInfo = helper(tree->right);
  auto isBalanced = leftInfo.balanced && rightInfo.balanced &&
                    abs(leftInfo.height-rightInfo.height)<=1;
  auto height = 1+max(leftInfo.height,rightInfo.height);
  return TreeInfo(isBalanced, height);
}
bool heightBalancedBinaryTree(BinaryTree *tree) {
  // Write your code here.
 TreeInfo info = helper(tree);  
 return info.balanced;
}

#include <vector>
using namespace std;

class BinaryTree {
public:
  int value;
  BinaryTree *left;
  BinaryTree *right;

  BinaryTree(int value);
  void insert(vector<int> values, int i = 0);
};

//O(n) time and O(log(n)) space
vector<int> findMaxPath(BinaryTree *tree){
  if(tree==NULL){
    return vector<int>{0, INT_MIN};
  }

  vector<int> leftMax = findMaxPath(tree->left);
  vector<int> rightMax = findMaxPath(tree->right);

  int leftMaxPathSum = leftMax[1];
  int rightMaxPathSum = rightMax[1];
  int leftMaxBranchSum = leftMax[0];
  int rightMaxBranchSum = rightMax[0];

  //max of left or right branch
  int maxBranchSum = max(leftMaxBranchSum, rightMaxBranchSum);
  
  //max of left or right branch plus current node or current node itself
  int maxBranchSumWithRoot = max(maxBranchSum+tree->value, tree->value);
  
  //max of possible path with left + righ paths plus current node and max branch with root
  int currentMaxPathSum = max(leftMaxBranchSum+tree->value+rightMaxBranchSum, maxBranchSumWithRoot);

  //max of left max path or right max path or current max path
  int maxPathSum = max(max(leftMaxPathSum, rightMaxPathSum), currentMaxPathSum);
  
  return vector<int>{maxBranchSumWithRoot, maxPathSum};
}

int maxPathSum(BinaryTree tree) {
  vector<int> res = findMaxPath(&tree);
  return res[1];
}

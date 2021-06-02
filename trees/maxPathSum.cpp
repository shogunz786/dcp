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

vector<int> maxPathSumHelper(BinaryTree *tree){
	if(tree == NULL)
		return {0,INT_MIN};
	vector<int> left = maxPathSumHelper(tree->left);
	vector<int> right = maxPathSumHelper(tree->right);
	int maxChildSumAsBranch=max(left[0],right[0]);
	int maxSumAsBranch=max(maxChildSumAsBranch+tree->value, tree->value);
	int maxSumAsRootNode = max(left[0]+tree->value+right[0],maxSumAsBranch);
	int maxPathSum=max(left[1],max(right[1],maxSumAsRootNode));
	return {maxSumAsBranch, maxPathSum};
}

//O(n) time and O(logn) space
int maxPathSum(BinaryTree tree) {
  // Write your code here.
	vector<int> ret = maxPathSumHelper(&tree);
  return ret[1];
}

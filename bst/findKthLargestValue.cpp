using namespace std;

class BST {
public:
  int value;
  BST *left = nullptr;
  BST *right = nullptr;

  BST(int value) { this->value = value; }
};

void inOrderTrav(BST* tree, vector<int> &array, int k){
  if(tree == NULL || k==0){
    return;
  }
  inOrderTrav(tree->right, array, k);
  k--;
  array.push_back(tree->value);
  inOrderTrav(tree->left, array, k);
}


//O(n) time and O(n) space
int findKthLargestValueInBst(BST *tree, int k) {
  vector<int> array;
  inOrderTrav(tree, array, k);
  return array[k-1];
}

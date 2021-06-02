using namespace std;

class BST {
public:
  int value;
  BST *left;
  BST *right;

  BST(int value) {
    this->value = value;
    left = NULL;
    right = NULL;
  }

  void insert(int value) {
    if (value < this->value) {
      if (left == NULL) {
        left = new BST(value);
      } else {
        left->insert(value);
      }
    } else {
      if (right == NULL) {
        right = new BST(value);
      } else {
        right->insert(value);
      }
    }
  }
};

BST* helper(vector<int> &array, int l, int r){
	if(l>r){
	  return NULL;	
	}
	int m = l+(r-l)/2;
	BST *root = new BST(array[m]);
	root->left = helper(array,l,m-1);
	root->right = helper(array,m+1,r);
	return root;
}

//O(nlogn) time and O(n) space
BST *minHeightBst(vector<int> array) {
  // Write your code here.
	sort(array.begin(), array.end());
  return helper(array,0, array.size()-1);
}

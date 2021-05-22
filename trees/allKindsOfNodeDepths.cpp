/*
  find sum of all of its subtrees nodes depths.
	input:
	          1
	     2         3
   4      5   6     7
8     9

output: 26
*/

 #include<stack>

using namespace std;

class BinaryTree {
public:
  int value;
  BinaryTree *left;
  BinaryTree *right;

  BinaryTree(int value) {
    this->value = value;
    left = nullptr;
    right = nullptr;
  }
};

int findNodeDepth(BinaryTree *bt, int depth=0){
	if(bt==NULL){
		return 0;
	}
	//add node depth of left + node depth of right +
	//node depth of the root
	//keep track of the node depth in each recursion stack
	return findNodeDepth(bt->left, depth+1)+
		     findNodeDepth(bt->right, depth+1)+depth;
}

//O(nlogn) time and O(h) space - n number of nodes and 
//h - height of the tree
int allKindsOfNodeDepths1(BinaryTree *root) {
	stack<BinaryTree*> st;
	st.push(root);
  int sum=0;
	
	while(!st.empty()){
		//add each node of the tree to the stack
		BinaryTree *bt = st.top();
		st.pop();
		if(bt==NULL){
			continue;
		}
		//find node depth of each node and accumulate sum
		sum+=findNodeDepth(bt);
		st.push(bt->left);
		st.push(bt->right);
	}
  return sum;
}

//O(nlogn) time and O(h) space - n - number of nodes
//h - height of the tree
int allKindsOfNodeDepthsRec(BinaryTree* root){
	if(root==NULL){
		return 0;
	}
	return allKindsOfNodeDepthsRec(root->left)+
		     allKindsOfNodeDepthsRec(root->right)+
		     findNodeDepth(root);
}

//O(n) time and O(h) space
int allKindsOfNodeDepths2(BinaryTree* bt, int depth=0){
	if(bt==NULL){
		return 0;
	}
	//node depth formula 1+2+3+4+5..+depth-1+depth
	auto depthSum = depth*(depth+1)/2;
	return depthSum + allKindsOfNodeDepths2(bt->left, depth+1)+
		     allKindsOfNodeDepths2(bt->right, depth+1);
}

//O(n) time and O(h) space
int allKindsOfNodeDepths3(BinaryTree *bt, int depthSum=0, int depth=0){
	if(bt==NULL){
		return 0;
	}
	depthSum+=depth;
	return depthSum+
		     allKindsOfNodeDepths3(bt->left, depthSum, depth+1)+
		     allKindsOfNodeDepths3(bt->right, depthSum, depth+1);
}

struct TreeInfo {
	int numNodesInTree;
	int sumOfDepths;
	int sumOfAllDepths;
};

//O(n) time and O(h) space
TreeInfo helper(BinaryTree *bt){
	if(bt==NULL){
		return TreeInfo{0,0,0};
	}
	//get Left TreeInfo
	TreeInfo leftInfo = helper(bt->left);
	//get Right TreeInfo
	TreeInfo rightInfo = helper(bt->right);
	//track left sum of depths
	int leftSumDepths = leftInfo.sumOfDepths+leftInfo.numNodesInTree;
	//track right sum of depths
	int rightSumDepths = rightInfo.sumOfDepths+rightInfo.numNodesInTree;
	//track total number of nodes
	int numOfNodes = 1 + leftInfo.numNodesInTree + rightInfo.numNodesInTree;
	
	//sum of right and left depths
	int sumOfDepths = leftSumDepths+rightSumDepths;
	//sum all depths
	int sumOfAllDepths =
		sumOfDepths+leftInfo.sumOfAllDepths+rightInfo.sumOfAllDepths;
	
	return TreeInfo{numOfNodes,sumOfDepths,sumOfAllDepths };
	
}

int allKindsOfNodeDepths(BinaryTree *bt){
	TreeInfo t = helper(bt);
	return t.sumOfAllDepths;
}


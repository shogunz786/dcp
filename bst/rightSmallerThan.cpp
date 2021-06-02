using namespace std;

class BST{
 public:
	int value;
	int leftSubTreeSize;
	BST *left, *right;
	
	BST(int value){
		this->value = value;
		left=right=NULL;
		leftSubTreeSize=0;
	}
	void insert(int value, int idx, int noSmaller, vector<int> &smallerArray, string str){
	  if(value<this->value){
			leftSubTreeSize++;
		  cout<<str<<"this->value="<<this->value<<" this->leftSz="<<this->leftSubTreeSize<<endl;
			//cout<<str<<"left idx="<<idx<<", val="<<value<<" leftSz="<<leftSubTreeSize<<endl;
			if(this->left==NULL){
		    this->left = new BST(value);		
			  smallerArray[idx]=noSmaller;	
			}else{
			  this->left->insert(value, idx, noSmaller, smallerArray, str+"  ");	
			}
		}else{
			noSmaller+=leftSubTreeSize;
			if(value>this->value){
				noSmaller++;
			  cout<<str<<"right idx="<<idx<<", val="<<value<<" noSmaller="<<noSmaller<<endl;
			}
			if(this->right==NULL){
		    this->right = new BST(value);		
			  smallerArray[idx]=noSmaller;	
			}else{
			  this->right->insert(value, idx, noSmaller, smallerArray, str+"  " );	
			}
		}	
	}
};

//O(nlogn) time and O(n) space - best case
//O(n^2) time and O(n) space  - worst case
vector<int> rightSmallerThan(vector<int> array) {
	if(array.size()==0){
		return {};
	}
  // Write your code here.
	vector<int> smallerArray(array.size(),0);
	BST *bst = new BST(array[array.size()-1]);
	for(int i=array.size()-2; i>=0; i--){
		bst->insert(array[i], i, 0, smallerArray, "");
		cout<<endl;
	}
  return smallerArray;
}

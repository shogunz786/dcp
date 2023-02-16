using namespace std;

class BST {
public:
 int value;
 BST *right;
 BST *left;
 int leftSubTreeSize;

 BST(int val){
   value = val;
   right = nullptr;
   left = nullptr;
   leftSubTreeSize = 0;//keep track of number of nodes in left sub tree
 }

//O(nlog(n)) time and O(n) space best case - well balanced BST
//O(n^2) time and O(n) space worst case - bst is like a linked list

//rightsmaller = sum of nodes on the left sub tree + add 1 if current value is greater
//than the node at which its inserted
 void insert(int val, int i, vector<int> &rightSmallerCounts, string str, int numSmallerAtInsert=0){
   if(val<value){
     //keep track of left tree size at each node
     leftSubTreeSize++;

     if(left == nullptr){
       left = new BST(val);
       //on new element insert capture the number of smaller elements
       rightSmallerCounts[i] = numSmallerAtInsert;
     }else{
       left->insert(val, i, rightSmallerCounts, str+" ", numSmallerAtInsert);
     }
   }else{
      //add number of nodes on the left sub tree that are smaller
      numSmallerAtInsert += leftSubTreeSize;
 
     if(val>value){
       //when val is greater than node value at which it's inserted increment
       numSmallerAtInsert++;
     }

     if(right == nullptr){
       right = new BST(val);
       rightSmallerCounts[i] = numSmallerAtInsert;
     }else{
       right->insert(val, i, rightSmallerCounts, str+" ", numSmallerAtInsert);
     }
   }
 }
};


vector<int> rightSmallerThan(vector<int> array) {
  if(array.size()==0){
    return {};
  }
  int lastIdx = array.size()-1;
  vector<int> res = array;
  BST *tree = new BST(array[lastIdx]);
  //no elements after the right most element
  res[lastIdx]=0;
  //insert from right
  for(int j=array.size()-2; j>=0; j--){
    tree->insert(array[j], j, res, "");
  }

  return res;
}

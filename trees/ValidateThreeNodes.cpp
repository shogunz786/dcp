using namespace std;

//prove one is parent of two and two is parent of three
//or three is parent of two and two is parent of one
class BST {
public:
  int value;
  BST *left = nullptr;
  BST *right = nullptr;

  BST(int value) { this->value = value; }
};

//O(h) time and O(h) space when using
bool isChild(BST *parent, BST *child){
  //found child success
  if(parent == child){
    return true;
  }

  //child not found failed
  if(parent==NULL){
    return false;
  }

  //travel to child
  parent = parent->value > child->value ? parent->left: parent->right;

  //recurse
  return isChild(parent, child);
}

//O(h) time and O(1) space when using
bool isChildIter(BST *parent, BST* child){
  while(parent != nullptr && parent != child){
     parent = parent->value > child->value ? parent->left: parent->right;
  }
  return parent == child;
}

// two is middle of the order
//either one or three is parent or one or three is child
//two is in the middle
bool validateThreeNodes1(BST *nodeOne, BST *nodeTwo, BST *nodeThree) {
  //two is one's parent 
  if(isChildIter(nodeTwo, nodeOne))
    //three is two's parent
    return isChildIter(nodeThree, nodeTwo);

  //two is three's parent
  if(isChildIter(nodeTwo, nodeThree))
    //one is two's parent
    return isChildIter(nodeOne, nodeTwo);

  //invalid order
  return false;
}

//O(d) time and O(1) space
bool validateThreeNodes(BST *nodeOne, BST *nodeTwo, BST *nodeThree) {
  BST *oneToTwo = nodeOne;
  BST *threeToTwo = nodeThree;
  bool foundTwo = false;
  bool finished = false;
  bool oneToThree = false;
  while(true){
    //reached two from one or three
    foundTwo = oneToTwo == nodeTwo || threeToTwo == nodeTwo;
    //did not find two
    finished = oneToTwo == nullptr && threeToTwo == nullptr;
    //did not find two between one and three
    oneToThree = oneToTwo == nodeThree || threeToTwo == nodeOne; 
    if(foundTwo || finished || oneToThree){
      break;
    }

    //travel from one to two
    if(oneToTwo){
      oneToTwo = oneToTwo->value > nodeTwo->value ? oneToTwo->left: oneToTwo->right;
    }

    //travel from three to two
    if(threeToTwo){
      threeToTwo = threeToTwo->value > nodeTwo->value ? threeToTwo->left: threeToTwo->right;
    }
  }

  //two not found
  if(oneToThree || !foundTwo){
    return false;
  }
  BST *child = nodeOne;
  if(oneToTwo == nodeTwo){
    child =  nodeThree;
  }
  //found parent of two need to find if child of two exists
  return isChildIter(nodeTwo, child);
}

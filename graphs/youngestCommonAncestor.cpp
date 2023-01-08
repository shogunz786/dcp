#include <vector>
using namespace std;

class AncestralTree {
public:
  char name;
  AncestralTree *ancestor;

  AncestralTree(char name) {
    this->name = name;
    this->ancestor = nullptr;
  }

  void addAsAncestor(vector<AncestralTree *> descendants);
};

int findDepth(AncestralTree *root, AncestralTree *one){
    int depth = 0;
    while(root!=one){
      depth++;
      one=one->ancestor;
    }
    return depth;
}

AncestralTree *findCommonAncestor(int diff, AncestralTree *big, AncestralTree *small){
  while(diff){
    big = big->ancestor;
    diff--;
  }
  while(big!=small){
        big = big->ancestor;
        small = small->ancestor;
  }
  return big;
}

AncestralTree *getYoungestCommonAncestor(AncestralTree *topAncestor,
                                         AncestralTree *descendantOne,
                                         AncestralTree *descendantTwo) {
  int oneDepth = findDepth(topAncestor, descendantOne);
  int twoDepth = findDepth(topAncestor, descendantTwo);
  if(oneDepth>twoDepth){
    return findCommonAncestor(oneDepth-twoDepth, descendantOne, descendantTwo);
  }else{
      return findCommonAncestor(twoDepth-oneDepth, descendantTwo, descendantOne);
  }
}

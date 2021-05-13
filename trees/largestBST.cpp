struct BSTInfo{ int max,min,isbst,sz;//size
  BSTInfo(){ max = INT_MIN; min = INT_MAX; isbst=1; sz = 0; }
};
BSTInfo *largestBST(Node *root)
{
  BSTInfo *curInfo = new BSTInfo();
  if(!root) return curInfo;
  BSTInfo *leftInfo = largestBST(root->left);
  BSTInfo *rightInfo = largestBST(root->right);
  if(!leftInfo->isbst || !rightInfo->isbst || /*left max should be less than cur*/
  leftInfo->max >root->data || rightInfo->min <= root->data){/*right min should be > cur*/
     curInfo->isbst = 0;//false
     curInfo->maxbst = max(lefInfo->sz, rightInfo->sz); 
     return curInfo;
  }
  curInfo->maxbst = 1+leftInfo->sz+rightInfo->sz;
  curInfo->max=cur->right?rightInfo->max:cur->data;//if right leaf present use right max 
  curInfo->min = cur->left? leftInfo->min: cur->data; 
  return curInfo;
}

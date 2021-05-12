struct node{
 int value;
 struct node *next;
};
//2->1 + 3->5 = 5->6
int getSize(struct node *n){
  int len = 0;
  while(n)
  {
    len++;
    n=n->next;
  }
  return len;
}
void addNode(node **root, int value)
{
  (*root) = (node*)malloc(sizeof(node));
  (*root)->next = NULL;
  (*root)->data = value;
}
node *add(node *n, node *iter)
{
  node *nlist = NULL;
  node **root = &nlist;
  node *ret = n, *prev = NULL;
  int carry=0, sum=0;
  while(iter){//go over shorter list and add to longer list
    sum = iter->data+res->data+carry;
    carry = sum/10;
    addNode(root,sum%10);
    root = &((*root)->next);
    iter = iter->next;
    prev = ret;
    ret = ret->next;
  } 
  if(carry)//still carry
  {
    while(ret && carry){//either carry or longer list exhausts
      sum = ret->data+carry;
      carry = sum/10;
      addNode(root,sum%10);
      root = &((*root)->next);
      prev = ret;
      ret = ret->next;     
    }
    if(carry)//still carry add new node
    {
      addNode(root,carry);
    }   
  }
  return n;
}
struct node* addNumbers(struct node* one, struct node* two){
  int one_len = getSize(one);
  int two_len = getSize(two);
  if(one_len==0)
    return two;
  if(two_len==0)
    return one;
  node *res = NULL;
  node *iter = NULL;
  if(one_len>two_len)
  {
     res = one;
     iter = two
  }else{
     res = two;
     iter = one;
  }

  return add(res, iter); 
}

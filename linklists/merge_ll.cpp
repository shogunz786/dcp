struct Node {
 int data;
 struct Node* next;
};

void twoHalves(Node *root, Node **one, Node **two)
{
  Node *fast = root?root->next:root;//start with next
  Node *slow = root;
  while(fast)
  {
     fast = fast->next;
     if(fast)
     {
       fast = fast->next;
       slow = slow->next;
     }
  }
    *two = slow->next;
    slow->next = NULL;
   *one = root;  
}

Node* merge(Node *a, Node *b)
{
  Node *ret = NULL, *temp = NULL;
  if(a == NULL)
   return b;
  if(b==NULL)
   return a;
  
  if(a->data<=b->data){
       ret = a;
       ret->next = merge(a->next,b);     
  }else{
       ret = b;
       ret->next = merge(a, b->next);     
  }
  return ret;
}

Node* mergeSort(Node *root)
{
   Node *one=NULL, *two=NULL;

   if(root==NULL | root->next ==NULL)
     return root;
   twoHalves(root, &one, &two);
   Node *a = mergeSort(one);
   Node *b = mergeSort(two);
   return merge(a,b);   
}

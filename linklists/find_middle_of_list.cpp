struct ll{
  int data;
  struct ll* next;
};

int findMiddle(struct ll* root)
{
  //check root and root->next for null
  if(root == NULL)
     return -1;
  if(root->next == NULL)
     return root->data;
  struct ll *slow = root;
  struct ll *fast = root->next->next;
  while(fast)
  {
    slow = slow->next;
    if(fast->next)
      fast = fast->next->next;
    else
      fast = fast->next;
  }
  return slow->data;
}

Node *findMiddle(Node *root)
{
   if(root==NULL || root->next ==NULL)
     return root;
   Node *slow = root;
   Node *fast = root->next;
   while(fast && slow)
   {
       fast = fast->next;
       if(fast)
       {
          fast = fast->next;
          slow = slow->next;
       }
   }
   return slow;
}

struct ll{
  int data;
   struct ll* next;
};
struct ll* zip(struct *root)
{
  //check root null
  if(root==NULL)
     return NULL;

  //check next of root for null 
  if(root->next == NULL)
      return root;
  
  struct ll *slow = root;
  struct ll *fast = root->next->next;

  //find middle of the list
  while(fast){
    slow = slow->next;
    if(fast->next == NULL)
       fast = fast->next->next;
    else
       fast = fast->next;
  }
  
  struct ll *first = root;
  struct ll *second = slow->next;

  //split the ll
  slow->next = NULL;
  
  struct ll *prev = NULL;
  //reverse the list
  while(second){
    cur = second;
    second = second-next;
    cur->next = prev;
    prev = cur;
  }
  //merge two lists
  struct ll *temp = NULL;
  while(cur){
     temp = first;
     first = first->next;
     temp->next = cur;
     cur = cur->next;
     temp->next->next = first;    
  } 
  return root;
}
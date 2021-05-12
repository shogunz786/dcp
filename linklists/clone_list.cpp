struct Node{
 int data;
 struct Node* next;
 struct Node* random;
 Node(int d):data(d){}
};

struct Node* clonelist(struct Node *root)
{
  struct *cur = root, *temp =NULL, *dup =NULL;

  if(root==NULL || root->next)
    return cur;

  //insert new node
  while(cur){
    temp = cur->next;
    cur->next = new Node(cur->data);
    cur->next->next = temp;//insert new node
    cur->next->random = NULL;
    cur = temp;
  }

  //set the random ptrs
  cur=root;
  while(cur){
   cur->next->random = cur->random->next;
   cur = cur->next? cur->next->next: cur->next;//skip to next old node
  }

  //reset the pointers
  cur=root;
  temp = root->next;
  dup = root->next;

  while(cur && dup){
   cur->next = cur->next? cur->next->next: cur->next;//link old to old nodes
   dup->next = dup->next? dup->next->next: dup->next;//link new to new nodes
   cur = cur->next;
   dup = copy->next;
  }
  return temp;  
}
/*
 * Find the median of a sorter linked list
 */
class Node{
  public:
   Node(int d){val=d; next=NULL;}
   int val;
   Node *next;
};

int findMedian(Node *root)
{
  if(!root)
    return -1;
  int count = 0;
  Node *small = NULL;
  Node *head = root;
  while(root != head){
  //if all nodes are equal start will be small
  //count all nodes
  //identify the smallest
     count++;
     if(!small)
      small = root;
     else if(small->val > root->val)
      small = root;
     root = root->next;
  }
  if(count%2==0) count = count/2;
  else count = (count/2)+1
  while(count){//go half way to find median
    small = small->next;
    count--;
  }
  return small->val;
}

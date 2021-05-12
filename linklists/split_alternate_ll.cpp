Move dest to the next of the current src node
void moveNode(struct Node **dest, struct **src)
{
  struct Node *n = *src;
  if(n==NULL)
    return;
  *src = n->next;//go to next src
  n->next = *dest;//set next of current node to dest
  //dest start with NULL
  *dest = n;//point dest to head
}
void splitAlternate(struct Node *source,struct Node **one,struct Node **two)
{
  struct Node *a = NULL;
  struct Node *b = NULL;
  if(source == NULL)
    return;
  while(source)
  {
     moveNode(&a, &source);
     if(source)
     moveNode(&b, &source);
  }
  *one = a;
  *two = b;
}
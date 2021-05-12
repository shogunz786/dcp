class Node{
 private:
  int key;
  int level;
  struct Node** forward;
 public:
  Node(int k, int level);
}
Node::Node(int k, int level)
{
  this.level = level
  key = k;
  forward = new Node*[level+1];
  memset(forward, 0, sizeof(Node*)*(level+1));
}

class SkipList{
 private:
  int max_level;
  int cur_level;
  float p;//probability, ideal is square root of n nodes as we go higher up
  Node *header;
 public:
  SkipList(int mlevel, float probability);
  void display();
  void insert(int k);
  int findLevel();
  Node* createNode(int k, int level); 
};

SkipList::SkipList(int mlevel, float prob)
{
  max_level = mlevel;
  p = prob;
  header = createNode(-1, mlevel);
}

Node* SkipList::createNode(int k, int level)
{
  Node *n = new Node(k, level);
  return n
}

Node* SkipList::findLevel()
{
  float f = (float)rand()/RAND_MAX;
  int level = 0;
  while(f<p && level<max_level)
  {
    f = (float)rand()/RAND_MAX;
    level++;
  }
  return level; 
} 

void SkipList::display(){
 for(int i=0; i<=max_level; i++)
 {
   Node *temp = header->forward[i];
   while(temp)
   {
     cout<<temp->key<<endl;
     temp = temp->forward[i];
   }
 }  
}

void SkipList::insert(int k)
{
   Node *cur = header;
   Node *updates[max_level+1];//check only current levels
   memset(updates, 0, sizeof(Node*)*(max_level+1))
  //find the level that need to be updated
  //start from top and scan down
  for(int i=cur_level; i>=0; i--)
  {
      while(cur->forward[i] != NULL &&
            cur->forward[i]->key < k)
         cur = cur->forward[i];//go to next only if less than k
      updates[i] = cur;
  }

  //reached zero level scanning down
  //go to the actual location
  cur = cur->forward[0];
  if(cur==NULL || cur->key != k)
  {
    int level = findLevel();
    //when random level > current level make sure to update the new levels
    for(int i=cur_level+1; i<=level; i++)
    {
       updates[i] = header;
    }
    if(cur_level<level)
       cur_level = level;
    Node *n = createNode(k, level);
    for(int i=0; i<=level; i++){
        n->forward[i] = updates[i]->forward[i];
        updates[i]->forward[i] = n;
    } 
  }

  //go to the actual location
  //location is null or not equal to key
}


Node* SkipList::find(int k)
{
   Node *cur = header;
   for(int j=cur_level; j>=0; j--)
   {
      while(cur->forward[j] != NULL &&
       cur->forward[j]->key < k)
         cur = cur->forward[j];
      if(cur->forward[j] != NULL && cur->forward[j]->key == k)
         return cur->forward[j];     
   } 
   return NULL;
}

void SkipList::delete(int k)
{
   Node *updates[cur_level+1];
   memset(updates, 0, sizeof(Node*)*(cur_level+1));
   Node *cur = header;
   Node *n = find(k);

   if(n == NULL)
     return;
 
   for(int j=n->level; j>=0; j--)
   {
      while(cur->forward[j] != NULL &&
       cur->forward[j]->key < k)
         cur = cur->forward[j];
      updates[j]=cur;
   }

   for(int i=n->level; i>=0; i--)
   {
      updates[i]->forward[i] = n->forward[i]; 
   }
}

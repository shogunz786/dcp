struct ll{
  int data;
  struct ll* next;
};

struct ll* reverse(struct ll* root)
{
   struct ll *cur=NULL, *prev = NULL;

   while(root)
   {
      cur = root;
      root =root->next;
      cur->next = prev;
      prev = cur;
   } 
   return cur;
}

struct ll* rev_groups(struct ll* root, int k)
{
  struct ll* res = NULL, *res_end;
  struct ll* cur = root;
  struct ll* start = NULL;
  while(cur)
  {
    start = cur;//get start of the list to reverse
    //iterate till k-1 one nodes 
    for(int i=0; i<k-1 && cur; i++){
       cur = cur->next;  
    }
    //if cur is not null prepare for next k nodes
    if(cur){
       end = cur;
       cur = cur->next;
       end->next = NULL;//terminate the list be reversed
    }
    if(res == NULL){//if first entry of the result list assign and init end of the list
        res = reverse(start);
    }else{
        res_end->next = reverse(start);
    }
    res_end = start;
  }
  return res;
}
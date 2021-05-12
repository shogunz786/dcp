void swapKth(struct LLNode** root, int k)
{
  if(k<=0)//check for valid len
     return;
  struct LLNode *cur=*root, *end = NULL,
                *prev_first = NULL, *prev_end=NULL,
                *first = NULL, *temp =NULL;
  int len = 0;
   while(cur){
    len++;
    cur = cur->next;
   } 
  if(k>len || (len+1)/2 == k)
  //check if k > len or end and first are same
    return;
  
  cur =*root;
  for(int i=1; i<k; i++)
  {
    prev_first = cur;//could be null if k == 1
    cur = cur->next;   
  } 
  first = cur;
  
  cur = *root;
  for(int i=1; i<len-k+1; i++)
  {
    prev_end = cur;//could be null if k==len
    cur = cur->next;
  }
  end = cur;

  if(first && end){//if first and end found
     temp = first->next;
     first->next = end->next;
     end->next = temp;
     if(prev_first){//first is nor head
       prev_first->next = end;
     }else{//first is head
       *root = end;
     }
     if(prev_end)//end is not head, when k > len/2
       prev_end->next = first;
     else//when end is head, when k>len/2
       *root = end; 
  }
}
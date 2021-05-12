class MinStack{
   stack<int> sdata;
   stack<int> aux;

   int getMinimum(){
       if(aux.empty())
           return -1;
       else
         return aux.top();
   }

   int push(int data)
   {
      if(aux.empty() || aux.top() >= data)
         aux.push(data));
      sdata.push(data);
   }

   int top()
   {
      if(sdata.empty())
         return -1;
       return sdata.top();
   }
 
   void pop(){
      if(!sdata.empty())
      {
        if(sdata.top() == aux.top())
             aux.pop()
        sdata.pop();        
      }
   }
};
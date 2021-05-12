class Sstack{
   private:
     int size;
     int *arr;
     int t;//top index
   public:
      void push(int i);
      void pop(int i);
      void inc(int first, int inc_by);
      Sstack(int sz){
        size = sz;
        arr = new int[sz];
        t = -1;
      }
      ~Sstack(){
        delete[] arr;
      }
      int top(){ return t;} 
};

void Sstack::push(int i)
{
  if((top+1) < size)
  {  
     top++;
     arr[top]=i;
  }
  cout<<arr[top]<<endl;
}

void Sstack::pop()
{
  if(top > -1)
    top--;
  if(top>=0)
    cout<<arr[top]<<endl;
  else
    cout<<"Empty"<<endl;
}

void inc(int first, int incby)
{
  for(int i=0; i<first && i<=top; i++)
  {
    arr[i]+=incby;    
  }
}

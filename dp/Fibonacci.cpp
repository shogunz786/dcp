unordered_map<int, int> cache;

//time O(n) and O(n) space
int fib(int n){
  if(n<=1){
    return n;
  }else{
    if(!cache.count(n)){
      cache[n] = fib(n-1)+fib(n-2);
    }
    return cache[n];
  }
}

//time O(n) and space O(1)
int fib(int n){
  if(n<=1){
    return n;
  }
  int f_2 = 0;//f-2
  int f_1 = 1;//f-1
  for(int i=2; i<=n; i++){
    int r = f_1+f_2;
    f_2=f_1;
    f_1=r;
  }
  return f_1;
}

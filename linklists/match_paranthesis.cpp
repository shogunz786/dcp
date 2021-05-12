bool checkValid(char a, char b)
{
  if((a='(' && b==')') ||
     (a='{' && b=='}') ||
     (a='[' && b==']'))
     return true;
  return false;     
}
bool validPara(string str)
{
  stack<char> sc;
  sc.push(-1);
  for(int i=0;i<str.length(); i++)
  {
    if(str[i]=='(' || str[i]=='{' || str[i]=='[')
      sc.push(str[i]);
    else{
      if(checkValid(sc.top(), str[i]))
          sc.pop();
      else
         return false;
    }
  }
  return (!sc.empty() && sc.top()==-1);
}
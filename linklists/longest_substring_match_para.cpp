int maxLenMatchingPattern(string str)
{
  stack<int> indices;
  int maxlen = 0;
  indices.push(-1);//marker to know start is from 0
  for(int i=0; i<str.length(); i++)
  {
    if(str[i]='(')
    {
       indices.push(i);
    }else if(str[i]=')'){
      indices.pop();
       if(!indices.empty())
            maxlen = max(maxlen, i-indices.top());
       else
          indices.push(i);
    }
  }
  return maxlen;
}
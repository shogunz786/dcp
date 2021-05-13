void reverseStr(string &input, int l, int r)
{
  if(l>=r)
     return;
  swap(str[l],str[r]);
  reverseStr(input, l+1, r-1);
}

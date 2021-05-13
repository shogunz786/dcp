string sortstr(string str)
{
  int p=0;
  int ch[256]={0};
  for(int i=0; i<str.length();i++)
      ch[str[i]]++;
  for(int j=0; j<256; j++)
  {
    for(int k=0; k<ch[j];k++)
    {
       str[p]=(char)j;
       p++;
    }
  }
  return str;
}
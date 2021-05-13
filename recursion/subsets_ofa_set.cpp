//index start with 0, and res is ""
void subsets(string str, int index, string res)
{
   if(index == str.length())
   {
     cout<<res<<endl;
     return;
   }
   subsets(str, index+1, res+str[index]);
   subsets(str, index+1, res);
}

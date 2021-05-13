void patternGen(vector<string>& res,string curExp, string input, int target, int pos, int curVal, int lastVal)
{
	//whole input is processed, may not be with all operators
	if(pos == input.length())
        {
	   //when matched with target capture for result
	   if(curVal == target)
	      res.push_back(curExp);
	   return;
        }

        for(int i=pos; i<input.length(); i++)
        {
	   string part = input.substr(pos, i+1-pos);
	   //ignore numbers prefixed with 0
	   if(part[0]=='0' && part.length()>1)
		   break;
	   int cur = atoi(part.c_str());
	   if(pos==0)
		   patternGen(res, curExp+part, input,
			target, i+1, cur, cur);
	   else{
		   patternGen(res, curExp+"+"+part, input,
			target, i+1, curVal+cur, cur);
		   patternGen(res, curExp+"-"+part, input,
			target, i+1, curVal-cur, -cur);
		   //to adjust precedence substract the lastVal from curVal to
		   //get orginal residue and then ass last*cur
		   //e.g: change from 1+2 to 1+2*3
		   patternGen(res, curExp+"*"+part, input,
			target, i+1, curVal-lastVal+lastVal*cur, lastVal*cur);
	   }
        }
}

vector<string> getExps(string input, int target)
{
   vector<string> res;
   patternGen(res, "", input, target, 0, 0, 0);
   return;
}

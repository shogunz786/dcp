int evalExp(string exp, map<char,int> order)
{
  stack<char> operators;
  stack<int> operands;
  int value = 0;
  for(int i=0; i<exp.length(); i++)
  {
    /*ignore spaces*/
    if(str[i]=' ')
	    continue;

    /*add left para to operands*/
    if(str[i]='(')
	  operators.push(str[i]);
    else if(str[i]=')')
    {
	    /*on right para, compute all operands and operators till
	     * left para is found*/
	    while(!operators.empty() && operators.top() != '(')
	    {
	       int op1 = operands.top();
	       operands.pop();
	       int op2 = operands.top();
	       operands.top;
	       value = compute(op1, op2, operators.top());
	       operators.pop();
	       operands.push(value);
	    }
	    //remove left para
	    operators.pop();
    }else if(order.find(str[i])!=order.end())
    {
	    /*compute all operands and operators when current 
	     * operator is less than or equal to operators.top() 
	     * in precedence
	     */
	    while(!operators.empty() && order[operators.top()]>=order[str[i]])
	    {
	       int op1 = operands.top();
	       operands.pop();
	       int op2 = operands.top();
	       operands.pop();
	       value = compute(op1, op2, operators.top());
	       operators.pop();
	       operands.push(value);
	    }
	    operators.push(str[i]);
    }else{
	  int j=i;
          while(j<str.length() && isdigit(str[j]))
		  j++;
	  operands.push(atoi(str.substr(i,j-i+1)));
	  i=j-1;
    }
  }
  while(!operators.empty())
  {
	  int op1 = operands.top();
	  operands.pop();
	  int op2 = operands.top();
	  operands.pop();
	  value = compute(op1, op2, operators.top());
	  operators.pop();
	  operands.push(value);
  }
  return operands.top();
}

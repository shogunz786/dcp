void next_palindrome(string &num, int l, int r, int carry)
{
	if((l<0||r>=num.length()))
	{//if carry present set first digit to 1 and prefix with num with 1
		if(carry)
		{
			num[num.length()-1]='1';
			num = '1'+num;
		}
		return;
	}else if(num[l]=='9|| num[r]=='9')
	{//if digit is 9 set num to 0 and pass carry to next level
		num[l]=num[r]='0';
		l--; r++;
		next_palindrome(num, l, r, 1)
	}else if(num[l]==num[r] && l!=r)
	{//if digit is 9 set num to 0 and pass carry to next level
		l--; r++;
		next_palindrome(num, l, r, 0)
	}else{//anything other than 9 increment the digit by 1
		num[l]=(char)((int)num[l]+1);   
		if(l!=r) num[r]=(char)((int)num[r]+1);//update r if its not middle element   
		l--; r++;
		next_palindrome(num, l, r, 1)
	}
}

bool is_palindrome(string num, int l, int r)
{
	while(l<r)
	{
            if(num[l++]!=num[r--])
		    return false;
	}
	return true;
}

void get_next_palindrome(int n)
{
	stringstream ostr;
	ostr<<n;
	string num = ostr.str();
	if(!is_palindrome(num, 0, num.length()-1)) return;
	int l=0, r=0;
	if(num.length()%2)
	{
		l=r=num.length()/2;
	}else{
		r=num.length()/2;
		l=r-1;
	}
	next_palindrome(num, l, r, 0);
	cout<<num<<endl;
}

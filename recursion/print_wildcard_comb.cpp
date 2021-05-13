void print_wild_card(string input, int i)
{
	if(i == input.length())
	{
		cout<<input<<endl;
		return;
	}
	if(input[i]=='?')
	{
		input[i] = '0';
		print_wild_char(input, i+1);
		input[i] = '1';
		print_wild_char(input, i+1);
		input[i] = '?';
	}else{
		print_wild_char(input, i+1);
	}
}

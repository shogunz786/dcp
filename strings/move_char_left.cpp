void move_char_left(string text)
{
	int begin_char=0;
	for(int i=0; i<text.length(); i++)
	{
		if(!(text[i]>='0' && text[i]<='9'))
		{
			if(begin_char!=i)
				text[begin_car]=text[i];
			begin_char++;
		}
	}
}

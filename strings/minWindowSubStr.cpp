string minWindowSubStr(string text, string chars)
{
	unordered_map<char, int> no_char_occurences_for_ref;//track number of occurences
	unordered_map<char, int> running _count_of_uniq_chars;//unique chars
	for(int i=0; i<chars.length();i++)
	{
		if(no_char_occurences_for_ref.find(chars[i])!=no_char_occurences_for_ref.end()){
			no_char_occurences_for_ref.insert(make_pair(chars[i],1));
			running _count_of_uniq_chars.insert(make_pair(chars[i],0));
		}else
			no_char_occurences_for_ref[chars[i]]++;
	}
	int count=chars.length(), begin=0, end =0, head=0;;
	int sz =INT_MAX;
	while(end<text.length())
	{
		if(no_char_occurences_for_ref.find(text[end])!=no_char_occurences_for_ref.end())
		{
			if(running _count_of_uniq_chars[text[end]]<=no_char_occurences_for_ref[text[end]])
				count--;
			running _count_of_uniq_chars[text[end]]++;
		}
		//found all chars capture len
		while(count==0)
		{//capture the window size and begin index
			if(no_char_occurences_for_ref.find(text[begin])!=no_char_occurences_for_ref.end())
			{
				running _count_of_uniq_chars[text[begin]]--;
				if(running _count_of_uniq_chars[text[begin]]<no_char_occurences_for_ref[text[begin]])
					count++;
			}
			if(sz>(end-begin+1))
			{
				sz = end-begin+1;
				head = begin;
			}//keep searching for next match 
			begin++;
		}
		end++;
	}
	return sz==INT_MAX?new string(""):text.substr(head, sz);
}

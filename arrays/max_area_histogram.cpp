int max_hist_area(vector<int> hist)
{
	stack<int> si;
	int i=0, tp_index=0, max_area=0, no_of_pillars=0;
	while(i<hist.size())
	{       //add to stack all heights >= top of stack
		if(si.empty() || hist[i]>=hist[si.top()]) si.push(i++);
		else{//reached height less than top
                   tp_index = si.top(); si.pop();//remove top and find its area
		   //when stack empty means no useful prior values so
		   //current index as number of pillars e.g 5,4,3,2
		   //else find the pillars between current and
		   //the last valid index in stack. e.g 4,5,6,3
		   //when at 3 i=3 and 5(i=1) is at top of stack
		   no_of_pillars = si.empty()?i:i-si.top()-1;
	           max_area =max(max_area, hist[tp_index]*no.of_pillars);
		}
	}
	while(!si.empty())
	{
                   tp_index = si.top(); si.pop();//remove top and find its area
		   no_of_pillars = si.empty()?i:i-si.top()-1;
	           max_area =max(max_area, hist[tp_index]*no.of_pillars);
	}
	return max_area;
}

void print_spiral(vector<<vector<int>> vvi)
{
	int c=0, r=0;
	int c_begin = 0, r_begin=0;
	int c_end=vvi.size(), r_end=vvi[0].size();
	//all columns visited ?
	while(c_begin<c_end && r_begin<r_end){
	   c=c_begin;
	   r=r_begin;
	   //go left
           while(c<c_end)
           {
		   cout<<vvi[r][c++]<<",";
	   }
	   r_begin++;//strike beginning row
	   if(r_begin>=r_end) break;//going of limits
	   r=r_begin;
	   c=c_end-1;
	   //go down
           while(r<r_end)
           {
		   cout<<vvi[r++][c]<<",";
	   }
	   c_end--;//strike the end column
	   if(c_begin>=c_end) break;//going of limits
	   c=c_end-1;
	   r=r_end-1;
	   //go right
           while(c>=c_begin)
           {
		   cout<<vvi[r][c--]<<",";
	   }
	   r_end--;//strike the end row
	   if(r_begin>=r_end) break;//going of limits
	   r=r_end-1;
	   c=c_begin;
	   //go up
	   while(r>=r_begin)
           {
		   cout<<vvi[r--][c]<<",";
	   }
	   c_begin++;//strike the begin column
	}
}

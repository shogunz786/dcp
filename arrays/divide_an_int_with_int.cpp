long divide(long a, long b)
{
	long shifts=0, q=0, neg=0;
	if(a==0) return 0;
	if(b==1) return a;
	if((a<0&&b>0) || (a>0&&b<0)) neg=1;
	a=abs(a); b=abs(b);
	//shift upto a is greater than or equal to shifted
	while((b<<(shifts+1))<=a) shifts++;
	while(a>=b)
	{
		while((b<<shifts)>a) shifts--;
                q +=(1LL<<shifts);//value of number of shifts
		a-=(b<<shifts);
	}
	return neg?-q:q;
}

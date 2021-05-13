float powerof(float base, int exp, int neg)
{
	if(exp == 0)
		return 1;
	if(exp%2){ 
		if(neg) 
			return powerof(base,(exp-1))* 1/base;
		else
			return powerof(base,(exp-1)) * base;
	}else{
		int res = power(base,exp/2);
		return res*res;
	}
}

float powFunc(float base, int exp)
{
	int neg = 0;
	if(exp<0)
		neg = 1;
	return powerof(base,-exp, neg);
}

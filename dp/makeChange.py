#/*
# * Given a set of denominations [1,2,5] and an amount.
# * Find all possible ways to change using the denominations.
# *
# */

#//time O(2^(A+D)) and space O(A+D) - A amount D no. of denominations
def numberOfWaysToMakeChangeRec(n, denoms):
    return helper(n,denoms,0)

def helper(n, denoms, dIdx):
	if(dIdx==len(denoms)):
		return 0
	if n == 0:
		return 1
	excl = helper(n,denoms,dIdx+1)
	incl=0
	if(n>=denoms[dIdx]):
		incl=helper(n-denoms[dIdx],denoms,dIdx)
	return excl+incl

#//time O(N) and space O(N)
def numberOfWaysToMakeChange(n, denoms):
	ways=[0]*(n+1)
	ways[0]=1
	for d in denoms:
		for val in range(1,n+1):
			if val>=d:
				ways[val]+=ways[val-d]
	return ways[n]

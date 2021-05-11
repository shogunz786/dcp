#/* Given a set of denominations and an amount
# * get the min coins required to make change.
# *
# */

#//time O(A*N) and space O(A) - A -amount and N - number of coins
def minNumberOfCoinsForChangeDP(n, denoms):
    totals=[n+1]*(n+1)
    totals[0]=0
	for d in denoms:
		for val in range(len(totals)):
			if val >= d:
				totals[val]=min(totals[val-d]+1,totals[val])
	if totals[n]>n:
		return -1
	else:
		return totals[n]

#//time O(A^N) and space O(A+N) - A -amount and N - number of coins
def minNumberOfCoinsForChange(n, denoms):
	if n<=0:
		return 0
	best=n+1
	for d in denoms:
		if n>=d:
			best = min(best,minNumberOfCoinsForChange(n-d, denoms)+1)
	return best if best <= n else -1

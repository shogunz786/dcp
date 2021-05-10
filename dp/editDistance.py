/* Given two strings "abc" and "yabd"
 * Find the minimum number of operations to change from first to second string * using operations like replace/delete/insert
 */

//time O(NM) and space(NM)
def levenshteinDistance(str1, str2):
	rows, cols = (len(str2)+1, len(str1)+1)
	dp = [[0 for i in range(cols)] for j in range(rows)]
	for j in range(0,len(str1)+1):
		dp[0][j]=j
	for i in range(0,len(str2)+1):
		dp[i][0]=i

    for i in range(1,len(str2)+1):
		for j in range(1,len(str1)+1):
			if str2[i-1] == str1[j-1]:
				dp[i][j]=dp[i-1][j-1]
			else:
				dp[i][j]= 1+min(dp[i-1][j-1],dp[i-1][j],dp[i][j-1])
	return dp[-1][-1]

//time O(NM) and space O(min(N,M))
def levenshteinDistance(str1, str2):
	big=str1
	small=str2
	if len(str1)<len(str2):
		big=str2
		small=str1
	cols = len(small)+1
	prev = [0 for i in range(cols)]
	cur = [0 for i in range(cols)]
	for j in range(0,cols):
		prev[j]=j
    for i in range(1,len(big)+1):
		cur[0]=i
		for j in range(1,len(small)+1):
			if big[i-1] == small[j-1]:
				cur[j]=prev[j-1]
			else:
				cur[j]= 1+min(prev[j-1],prev[j],cur[j-1])
		tmp=cur
		cur=prev
		prev=tmp
	return prev[-1]

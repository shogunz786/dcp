/* Given two strings "abc" and "yabd"
 * Find the minimum number of operations to change from first to second string * using operations like replace/delete/insert
 */

package main

//time O(NM) and space(NM)
func LevenshteinDistance1(a, b string) int {
	dp := make([][]int, len(b)+1)
	for x := range dp {
		dp[x] = make([]int, len(a)+1)
	}
	for i:=0;i<len(b)+1;i++ {
		dp[i][0]=i
	}
	for j:=0;j<len(a)+1;j++ {
		dp[0][j]=j
	}
	for i:=1;i<len(b)+1;i++ {
		for j:=1; j<len(a)+1; j++ {
			if a[j-1] == b[i-1] {
				dp[i][j]=dp[i-1][j-1]
			}else{
				dp[i][j]=1+min(dp[i-1][j],dp[i][j-1],dp[i-1][j-1])
			}
		}
	}
	return dp[len(b)][len(a)]
}

func min(args ...int) int {
	mn := args[0]
	for _,v := range args {
		if mn>v {
			mn=v
		}
	}
	return mn
}


//time O(NM) and space O(min(N,M))
func LevenshteinDistance(a, b string) int {
	big,small := a, b
	if len(a)<len(b){
     big,small = b, a
	}
	prev := make([]int, len(small)+1)
	cur := make([]int, len(small)+1)
	for i:=0; i<len(small)+1; i++ {
		prev[i]=i
	}
	for i:=1;i<len(big)+1;i++ {
		cur[0]=i
		for j:=1; j<len(small)+1; j++ {
			if small[j-1] == big[i-1] {
				cur[j]=prev[j-1]
			}else{
				cur[j]=1+min(prev[j],cur[j-1],prev[j-1])
			}
		}
		tmp := cur
		cur = prev
		prev = tmp
	}
	return prev[len(small)]
}

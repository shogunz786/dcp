/*
 * Given a set of denominations [1,2,5] and an amount.
 * Find all possible ways to change using the denominations.
 *
 */

package main

//time O(N) and space O(N)
func NumberOfWaysToMakeChangeDP(n int, denoms []int) int {
	ways:=make([]int,n+1)
	ways[0]=1
	for _,d := range denoms {
		for val:=1;val<n+1;val++ {
			if val>= d{
					ways[val]+=ways[val-d]
			}
		}
	}
	return ways[n]
}

//time O(2^(A+D)) and space O(A+D) - A amount D no. of denominations
func helper(n int, denoms[] int, dIdx int)int {
	if(dIdx==len(denoms)){
		return 0
	}
	if n==0 {
		return 1
	}
	excl := helper(n, denoms, dIdx+1)
	incl:=0
	if n >= denoms[dIdx] {
		incl=helper(n-denoms[dIdx], denoms, dIdx)
	}
	return excl+incl
}

func NumberOfWaysToMakeChange(n int, denoms []int) int {
	return helper(n, denoms, 0)
}

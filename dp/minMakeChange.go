/* Given a set of denominations and an amount
 * get the min coins required to make change.
 *
 */

package main


//time O(A^N) and space O(A+N) - A -amount and N - number of coins
func MinNumberOfCoinsForChangeRec(n int, denoms []int) int {
	if n<= 0 {
		return 0
	}
	best:=n+1
	for _,d := range denoms {
		if n>=d {
			best = min(best,MinNumberOfCoinsForChangeRec(n-d,denoms)+1 )
		}
	}
	if best > n {
		return -1
	}else{
		return best
	}
}

//time O(A*N) and space O(A) - A -amount and N - number of coins
func MinNumberOfCoinsForChangeDP(n int, denoms []int) int {
	totals := make([]int,n+1)
	totals[0]=0
	for i:=1;i<len(totals); i++ {
		totals[i]=n+1
	}
	for _,d := range denoms {
		for v := range totals {
			if v>=d {
				totals[v]=min(totals[v-d]+1,totals[v])
			}
		}
	}
	if totals[n]>n {
		return -1
	}else{
		return totals[n]
	}
}

func min(args ...int) int{
	v:=args[0]
	for _,a := range args {
		if v>a {
			v=a
		}
	}
	return v
}

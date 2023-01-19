package main
import "sort"

//O(nlogn) time and O(1) space
func NonConstructibleChange(coins []int) int {
	changeCreated :=0
    sort.Ints(coins)
    for _,coin := range coins {
        if coin > changeCreated+1 {
            return changeCreated+1
        }
        changeCreated += coin
    }
	return changeCreated+1
}

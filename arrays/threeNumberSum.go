package main
import "sort"

//O(n^2) time and O(n) space
func ThreeNumberSum(array []int, target int) [][]int {
	sort.Ints(array)
    res := [][]int{}
    for i:=0; i<len(array)-1; i++ {
        l, r := i+1,len(array)-1
      //for each i travel left and right in a sorted array to find target
        for l<r {
           sum := array[i]+array[l]+array[r]
            if sum == target {
                res = append(res, []int{array[i],array[l],array[r]})
                l++
                r--
            } else if target<sum {
                r--
            }else{
                l++
            }
        }
    }

	return res
}

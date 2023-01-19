package main
import "sort"

//O(n) time and O(n) space
func SortedSquaredArray1(array []int) []int {
    res := make([]int, len(array))
    i:=len(res)-1
	  l := 0
    r := len(array)-1
    for i>=0 {
        left := array[l]*array[l]
        right := array[r]*array[r]
        if left > right {
            res[i]=left
            l++
        } else {
            res[i]=right
            r--
        }
        i--
    }
	return res
}

//O(nlogn) time and O(1) space
func SortedSquaredArray2(array []int) []int {
    for i, num := range array {
        array[i] = num * num
    }

    sort.Ints(array)
	  return array
}

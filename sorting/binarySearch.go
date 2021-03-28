/*
 * Given a sorted array, use binary search to see if a element is present in the array.
 */

//time O(log(N) and space O(1))
package main

func BinarySearch(array []int, target int) int {
	l := 0
	r := len(array)-1
	for l<=r {
		m := (l+r)/2
		if array[m]==target {
			return m
		} else if array[m] > target {
			r = m-1
		}else{
			l = m+1
		}
	}
	return -1
}

//recursion
//time O(log(N)) and space O(log(N))
func BinarySearch1(array []int, target int) int {
	// Write your code here.
	return Helper(array,0,len(array)-1, target)
}

func Helper(array []int, left int, right int, target int) int {
	if left > right {
		return -1
	}
	m := (left+right)/2
	if array[m] == target {
		return m
	} else if array[m] > target {
		return Helper(array, left, m-1, target)
	}
	return Helper(array, m+1, right, target)
}

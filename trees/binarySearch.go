package main

//O(log(n)) time and O(1) space
func BinarySearchIter(array []int, target int) int {
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

func helper(array []int, target int, left int, right int) int {
	if left>right {
		return -1
	}
	mid := left+(right-left)/2
	if array[mid] == target {
		return mid
	} else if array[mid] > target {
		return helper(array, target, left, mid-1)
	} else {
		return helper(array, target, mid+1, right)
	}
}

//O(log(n)) time and O(log(n)) space
func BinarySearch(array []int, target int) int {
	return helper(array, target, 0, len(array))
}

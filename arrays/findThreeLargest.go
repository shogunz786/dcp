/*
 * Given an unsorted array, return the largest k elements
 * without sorting the array.
 *
 * input [10,5,9.10.12]
 * largest 3 elements
 * output [10,10,12]
 */
package main

import "math"

//O(n) time and O(1) space
func FindThreeLargestNumbers(array []int) []int {
	three_numbers := []int{math.MinInt32, math.MinInt32, math.MinInt32}
	for _,v := range array {
			updateLargest(three_numbers, v)
	}
	return three_numbers
}

func updateLargest(three_numbers []int, val int){
	if val > three_numbers[2] {
		updateAndShift(val, three_numbers, 2)
	}else if val > three_numbers[1] {
		updateAndShift(val, three_numbers, 1)
	}else if val > three_numbers[0] {
		updateAndShift(val, three_numbers, 0)
	}
}

func updateAndShift(val int, three_numbers []int, idx int){
	for i:=0; i < idx+1; i++ {
		if i==idx {
			three_numbers[idx]=val
		}else{
		  three_numbers[i]=three_numbers[i+1]
		}
	}
}

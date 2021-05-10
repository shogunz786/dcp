/*
 * Insertion sort of an array.
 *
 * first sort 0,1
 * next  sort 0,1,2
 * next  sort 0,1,2,3
 */

//worst & average time O(N^2) and space O(1)
//best time O(N) and space O(1)
package main

func InsertionSort(array []int) []int {
	if len(array) == 0 {
		return nil
	}
	for i:=range array{
		for j:=i; j>0 && array[j] < array[j-1];j-- {
				array[j-1], array[j]=array[j], array[j-1]
		}
	}
	return array
}


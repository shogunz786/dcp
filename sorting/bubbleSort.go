/*
 * Sort an array using bubble sort.
 *
 * Keep swapping neighbors from 0 to end of the array till
 * there is no need to swap to make lower index smaller than
 * the larget neighbor
 */

//averga and worst time O(N^2) and space O(1)
//best time O(N) and space O(1)
package main

func BubbleSort(array []int) []int {
	isSorted := false
	counter := 0
	for isSorted != true {
		isSorted=true
		for i:=0; i < len(array)-1-counter; i++ {
			if array[i]>array[i+1]{
				swap(i, i+1, array)
			}
			isSorted=false
		}
		counter++
	}
	return array
}

func swap( i int, j int, array []int){
	array[i], array[j] = array[j],array[i]
}

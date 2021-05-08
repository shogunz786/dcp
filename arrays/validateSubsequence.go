/*
 * Given a sequence A and sub sequence B. verify that the 
 * sub sequence is part of A in same order and does not
 * have to be adjacent to each other
 *
 * array [ 1,2,3,4]
 * sequence [1,3,4]
 * result is true
 */

//time O(N) and space O(1)
package main

//time O(n) | O(1) space
func IsValidSubsequence1(array []int, sequence []int) bool {
	aIndex:=0
	sIndex:=0
	for aIndex<len(array) && sIndex<len(sequence) {
		if array[aIndex]==sequence[sIndex] {
			sIndex+=1
		}
		aIndex+=1
	}
	return len(sequence)==sIndex
}

func IsValidSubsequence(array []int, sequence []int) bool {
	sIndex:=0
	for _, value := range array {
		if sIndex == len(sequence) {
			break
		}
		if value==sequence[sIndex] {
			sIndex+=1
		}
	}
	return len(sequence)==sIndex
}

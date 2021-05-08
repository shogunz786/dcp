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
def isValidSubsequence1(array, sequence):
	sIdx = 0
    for value in array:
		if len(sequence) == sIdx:
			break
		if value == sequence[sIdx]:
			sIdx+=1
	return sIdx == len(sequence)

def isValidSubsequence(array, sequence):
	sIdx = 0
	aIdx = 0
    while aIdx<len(array) and sIdx<len(sequence):
		if array[aIdx] == sequence[sIdx]:
			sIdx+=1
		aIdx+=1
	return sIdx == len(sequence)

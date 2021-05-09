#/*
# * Given an unsorted array, return the largest k elements
# * without sorting the array.
# *
# * input [10,5,9.10.12]
# * largest 3 elements
# * output [10,10,12]
# */

#//time O(N) and space O(k)
#O(n) time and O(1) space
def findThreeLargestNumbers(array):
	three_numbers = [None, None, None]
	for val in array:
		updateLargest(val, three_numbers)
	return three_numbers

def updateLargest(val, numbers):
	if numbers[2] is None or val > int(numbers[2]):
		moveRight(val, numbers, 2)
	elif numbers[1] is None or val > int(numbers[1]):
		moveRight(val, numbers, 1)
	elif numbers[0] is None or val > int(numbers[0]):
		moveRight(val, numbers, 0)

def moveRight(val, numbers, idx):
	i=0
	while i<=idx:
		if i == idx:
			numbers[idx]=val
		else:
			numbers[i]=numbers[i+1]
		i += 1

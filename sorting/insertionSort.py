#/*
# * Insertion sort of an array.
# *
# * first sort 0,1
# * next  sort 0,1,2
# * next  sort 0,1,2,3
# */
#
#//worst & average time O(N^2) and space O(1)
#//best time O(N) and space O(1)
def insertionSort(array):
	for i in range(1,len(array)):
		j=i
		while j>0 and array[j]<array[j-1]:
			array[j],array[j-1]=array[j-1],array[j]
			j -= 1
	return array



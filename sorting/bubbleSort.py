#/*
# * Sort an array using bubble sort.
# *
# * Keep swapping neighbors from 0 to end of the array till
# * there is no need to swap to make lower index smaller than
# * the larget neighbor
# */

#//averga and worst time O(N^2) and space O(1)
#//best time O(N) and space O(1)
def bubbleSort(array):
    isSorted=False
	counter=0
	while not isSorted:
		isSorted=True
		for i in range(len(array)-counter-1):
			if array[i]>array[i+1]:
				swap(i,i+1,array)
				isSorted=False
        counter += 1
	return array

def swap(i,j,array):
	array[i],array[j]=array[j],array[i]




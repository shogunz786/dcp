#O(log(n)) time and space O(1)
def binarySearchIter(array, target):
	l=0
	r=len(array)-1
	while l<=r:
		mid=l+(r-l)//2
		if array[mid]==target:
			return mid
		elif array[mid]>target:
			r=mid-1
		else:
			l=mid+1
	return -1

#O(log(n)) time and O(log(n)) space
def helper(array, target, left, right):
	if left>right:
		return -1
	mid=left+(right-left)//2
	if array[mid]==target:
		return mid
	elif array[mid]>target:
		return helper(array, target, left, mid-1)
	else:
		return helper(array, target, mid+1, right)

def binarySearch(array, target):
	return helper(array, target, 0, len(array)-1)

/*
 * Given an unsorted array and a target sum return elements that make up the target sum from the array.
 */

//time O(nlogn) and space O(1)
//sort array and check of begin element and end element sum match target
//if less move towards end else move end towards begin.
def twoNumberSum(array, targetSum):
	array.sort() 
	l=0 
	r=len(array)-1
	while l<r:
		sum=array[l]+array[r]
		if sum==targetSum:
			return  [array[l],array[r]]
		elif sum>targetSum:
			r-=1
		else:
		    l+=1
	return []

//time O(n) and space O(n)
//use a hashtable to lookup for the other number
def twoNumberSum(array, targetSum):
    # Write your code here.
	mp = {}
	for l in array:
		r = targetSum - l
		if r in mp:
			return [l,r]
		else:
		    mp[l]=1
	return []}

//time O(N^2) and space O(1)
def twoNumberSum(array, targetSum):
	for i in range(len(array)-1):
		first=array[i]
		for j in range(i+1,len(array)):
			second=array[j]
			if targetSum==first+second:
				return [first,second]
	return []

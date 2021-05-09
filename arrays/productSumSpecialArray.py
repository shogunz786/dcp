/*
 * Given special array [x, [y,[z,w]]] the product should return
 * x+2*(y+3*(z+w))
 *
 * use of any
 */

//time O(N) and space O(d), d - depth
def helper(array,level):
	sum=0
	for a in array:
	    if type(a) is int:
			sum+=a
		else:
		    sum+=helper(a,level+1)
	return sum*level

def productSum(array):
	return helper(array,1)

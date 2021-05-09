/*
 * Get nth Fibonacci number.
 */
 #O(2^n) time and O(n) space
def getNthFibRec(n):
    # Write your code here.
    if n == 1:
		return 0
	elif n == 2:
		return 1
	return getNthFib(n-1)+getNthFib(n-2)

def helper(n, mem):
	if n in mem:
		return mem[n]
	mem[n] = helper(n-1,mem)+helper(n-2,mem)
	return mem[n]

#O(2^n) time and O(n) space
def getNthFibRecMem(n):
	mem = { 1:0, 2:1 }
	return helper(n,mem)

//time O(n) and space O(1)
def getNthFib(n):
	if n == 1:
		return 0
	elif n == 2:
		return 1
	first=0
	second=1
	while n>2 :
		tmp = first+second
		first=second
		second=tmp
		n -=1
	return second

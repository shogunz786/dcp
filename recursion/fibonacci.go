/*
 * Get nth Fibonacci number.
 */
 package main

//time O(2^n) and space (n)
func GetNthFibRec(n int) int {
	if n == 1 {
		return 0
	} else if n == 2 {
	  return 1
	}
	return GetNthFib(n-1)+GetNthFib(n-2)
}

//time O(n) and space O(1)
func GetNthFibIter(n int) int {
	if n == 1 {
		return 0
	} else if n == 2 {
		return 1
	}
	first := 0
	second := 1
	tmp:=0
	for n>2 {
		tmp = first+second
		first = second
		second = tmp
		n -= 1
	}
	return second
}

//time O(n) and space O(n)
func helper(n int, mem map[int]int) int {
	if val, found := mem[n]; found {
		return val
	}
  mem[n]=helper(n-1, mem)+helper(n-2, mem)
	return mem[n]
}

func GetNthFibRecurMem(n int) int {
	return helper(n, map[int]int {
		1:0,
		2:1,
	})
}

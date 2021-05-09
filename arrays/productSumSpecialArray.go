/*
 * Given special array [x, [y,[z,w]]] the product should return
 * x+2*(y+3*(z+w))
 *
 * use of any
 */
package main

type SpecialArray []interface{}

//O(n) time and O(d) space - d is greatest depth
func helper(array SpecialArray, level int) int{
	sum:=0
	for _,a := range array{
		if val,ok := a.(SpecialArray); ok {
			sum += helper(val, level+1)
		}else if val,ok := a.(int); ok {
			sum += val
		}
	}
	return sum*level
}

func ProductSum(array []interface{}) int {
	return helper(array, 1)
}



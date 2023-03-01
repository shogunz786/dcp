/*
 * Given a BST and a value. Find the closest value in the tree to the given value.
 *
 */

class BST:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None


#Average case :O(log(n)) time and O(1) space
#Worst case: O(n) time and O(1) space
def findClosestValueInBstIter(tree, target):
    closest=tree.value
	while tree is not None:
		if abs(closest-target) > abs(tree.value-target):
			closest = tree.value
		if target > tree.value:
			tree = tree.right
		else:
			tree = tree.left
	return closest

def findClosestValueInBst(tree, target):
	return findClosestValueInBstRecur(tree, target, tree.value)

#Average case :O(log(n)) time and O(log(n)) space
#Worst case: O(n) time and O(n) space
def findClosestValueInBstRecur(tree, target, closest):
	if tree is None:
		return closest

	if abs(closest - target) > abs(tree.value - target):
		closest = tree.value
	if target > tree.value:
		closest = findClosestValueInBstRecur(tree.right, target, closest)
	else:
		closest=findClosestValueInBstRecur(tree.left, target, closest)

	return closest

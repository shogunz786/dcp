class BinaryTree:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

def branchSums(root):
    sums = list()
    branchSumHelper(root, 0, sums)
    return sums

#O(n) time and O(n) space
def branchSumHelper(root, cur_sum, sums):
    if root is None:
        return
    cur_sum += root.value
    if root.left is None and root.right is None:
        sums.append(cur_sum)
        return
    branchSumHelper(root.left, cur_sum, sums)
    branchSumHelper(root.right, cur_sum, sums)

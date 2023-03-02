#O(n) time and O(h) space
def nodeDepths1(root, depth=0):
    if root is None:
        return 0
    return depth+nodeDepths(root.left, depth+1)+nodeDepths(root.right, depth+1)

#O(n) time and O(h) space - h height of binary tree
def nodeDepths(root):
    sum_of_depths = 0
    stack = [{"node": root, "depth": 0}]
    while len(stack) > 0:
        nodeInfo = stack.pop()
        node, depth = nodeInfo["node"], nodeInfo["depth"]
        if node is None:
            continue
        sum_of_depths+=depth
        stack.append({"node": node.left, "depth": depth+1})
        stack.append({"node": node.right, "depth": depth+1})
    return sum_of_depths
    
# This is the class of the input binary tree.
class BinaryTree:
    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None

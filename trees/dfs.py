/*
 * Given a tree with n children per node, capture the dfs traversal in an array.
 */
 class Node:
    def __init__(self, name):
        self.children = []
        self.name = name

    def addChild(self, name):
        self.children.append(Node(name))
        return self

	//time O(v+e) and space O(v) - v-vertices and e -edges
	def depthFirstSearch(self, array):
	    array.append(self.name)
		for a in self.children:
			a.depthFirstSearch(array)
		return array

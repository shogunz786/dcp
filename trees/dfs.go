/*
 * Given a tree with n children per node, capture the dfs traversal in an array.
 */
package main

type Node struct {
	Name     string
	Children []*Node
}

//O(v+e) time and O(v) space - v-vertices and e-edges
func (n *Node) DepthFirstSearch(array []string) []string{
	array=append(array,n.Name)
	for _,child := range n.Children {
			array = child.DepthFirstSearch(array)
	}
	return array
}

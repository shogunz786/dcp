package main

type LinkedList struct {
	Value int
	Next  *LinkedList
}

func reverseList(root *LinkedList) *LinkedList{
    var prevNode *LinkedList
    currNode := root
    for currNode != nil {
        nextNode := currNode.Next
        currNode.Next = prevNode
        prevNode = currNode
        currNode = nextNode
    }
    return prevNode;
}

func splitList(oneNode *LinkedList) (*LinkedList, *LinkedList) {
    slow := oneNode
    fast := oneNode
    for fast != nil && fast.Next != nil {
        slow = slow.Next
        fast = fast.Next.Next
    }
    return slow, slow.Next
}

func zip(one *LinkedList, two *LinkedList) *LinkedList {
    var one_next, two_next *LinkedList
    root := one
    for one != nil && two != nil {
        one_next = one.Next
        two_next = two.Next
        one.Next = two
        two.Next = one_next
        two = two_next
        one = one_next
    }
    return root
}

//O(n) time and O(1) space
func ZipLinkedList(one *LinkedList) *LinkedList {
    slow, second := splitList(one)
    slow.Next = nil
    second = reverseList(second)
    
	return zip(one, second)
}

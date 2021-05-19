/*
 * input a->b->c->d
 * output b->a->d->c
 * swap adjacent nodes in a linked list
 */
using namespace std;

// This is an input struct. Do not edit.
class LinkedList {
public:
  int value;
  LinkedList *next = nullptr;

  LinkedList(int value) { this->value = value; }
};

//O(n) space and O(n) time
LinkedList *nodeSwap1(LinkedList *head) {
	//curr or next is null return
	if(head==NULL || head->next==NULL){
		return head;
	}
	//a->b->c->d
	//a=head
	//b=a->next
	//c=a->next->next
	LinkedList* a = head;
	LinkedList* b = a->next;
	//nodeSwap c recursively
	//a->d->c
  a->next = nodeSwap1(a->next->next);
	//swap a->b to b->a
	//b->a->d->c
	b->next = head;
  return b;
}

//O(n) time and O(1) space
LinkedList *nodeSwap(LinkedList *head) {
	//create a dummy node
	LinkedList* prevNode = new LinkedList(0);
	prevNode->next = head;
	LinkedList *tempRoot = prevNode;
  //check if current and current->next are valid
	while(prevNode->next!= NULL && prevNode->next->next != NULL){
		  //get first node
	   	LinkedList* one = prevNode->next; 
		  //get second node
			LinkedList* two = prevNode->next->next;
		  //point one next to next of two
		  one->next = two->next;
		  //point two next to one
		  two->next = one;
		  //make two as head
		  prevNode->next = two;
		  //set one as prev node to process remaining nodes
		  prevNode = one;
	}
	return tempRoot->next;
}


/*
  inverted bisection
	invert bisections of linked list
	
  input = 0->1->2->3->4->5
	output = 2->1->0->5->4->3
	
	input = 0->1->2->3->4->5->6
	output = 0->1->2->3->6->5->4
*/
using namespace std;

// This is an input class. Do not edit.
class LinkedList {
public:
  int value;
  LinkedList *next;

  LinkedList(int value) {
    this->value = value;
    this->next = nullptr;
  }
};

//find lenght of the list
int getLinkedListLen(LinkedList *head){
	int count=0;
	while(head){
		count++;
		head=head->next;
	}
	return count;
}

//reverse the linked list
LinkedList* reverseLL(LinkedList *head, LinkedList **secondList, int count){
	LinkedList *prev=NULL, *temp=NULL;
	while(head && count){
		  temp=head;
	    head = head->next;	
		  temp->next = prev;
		  prev = temp;
		  count--;
	}
	*secondList = head;
	return prev;
	
}

LinkedList *invertedBisection(LinkedList *head) {
	if(head==NULL || head->next==NULL){
		return head;
	}
	LinkedList *temp = NULL;
	LinkedList *two = NULL;
	LinkedList *middle = NULL;
	//capture head to use it as the end of the first list
	LinkedList *endOfOne = head;
	//find length of the list
	int len = getLinkedListLen(head);
	//reverse first half and get head of first list and second list
	LinkedList *one = reverseLL(head, &temp, len/2);
	int odd = len%2;
	
	//keep track of the middle
	middle = temp;
	temp = NULL;
	//if odd len use next of middle to reverse or use the middle if even
  two = reverseLL(odd?middle->next:middle, &temp, len/2);	
	if(odd){
		//if odd middle's next is second reversed list
	  middle->next = two;
		two = middle;
	}	
	//next of first list is second
	endOfOne->next = two;
  return one;
}

using namespace std;

class Node {
public:
  int value;
  Node *prev;
  Node *next;

  Node(int value);
};

class DoublyLinkedList {
public:
  Node *head;
  Node *tail;

  DoublyLinkedList() {
    head = nullptr;
    tail = nullptr;
  }

  void setHead(Node *node) {
    if(head==NULL){
      tail=head=node;
    }else{
      insertBefore(head, node);
    }
  }

  void setTail(Node *node) {
    if(tail==NULL){
      setHead(node);
    }else{
      insertAfter(tail, node);
    }
  }

  void insertBefore(Node *node, Node *nodeToInsert) {
    if(nodeToInsert == head && nodeToInsert==tail){
      return;
    }
    remove(nodeToInsert);
    Node* prev = node->prev;
    nodeToInsert->prev = prev;
    nodeToInsert->next = node;
    if(node==head){
      head = nodeToInsert;
    }else{
      prev->next = nodeToInsert;
    }
    node->prev = nodeToInsert;
  }

  void insertAfter(Node *node, Node *nodeToInsert) {
    if(nodeToInsert == head && nodeToInsert==tail){
      return;
    }
    remove(nodeToInsert);
    if(node==tail){
      tail->next = nodeToInsert;
      nodeToInsert->prev = tail;
      tail = nodeToInsert;
    }else{
      Node *next = node->next;
      node->next = nodeToInsert;
      nodeToInsert->prev = node;
      nodeToInsert->next = next;
      next->prev = nodeToInsert;
    }
  }

  void insertAtPosition(int position, Node *nodeToInsert) {
    Node *curr = head;
    position--;
    while(position){
      curr=curr->next;
      position--;
    }
    if(curr){
      insertBefore(curr, nodeToInsert);
    }else{
      setTail(nodeToInsert);
    }
  }

  void removeNodesWithValue(int value) {
    Node* curr = head;
    Node* last = NULL;
    while(curr){
      last = curr;
      curr=curr->next;
      if(last->value == value){
        remove(last);
      }
    }
  }

  void remove(Node *node) {
    if(node==head){
      head = node->next;
    }else if(node == tail){
      tail = tail->prev;
    }
    
    Node* prev = node->prev;
    Node* next = node->next;
    if(prev){
        prev->next = next;
    }

    if(next){
      next->prev = prev;
    }

    node->next = NULL;
    node->prev = NULL;
  }

  bool containsNodeWithValue(int value) {
    Node* curr = head;
    while(curr){
      if(curr->value==value){
        return true;
      }
      curr=curr->next;
    }
    return false;
  }
};

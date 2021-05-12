#include <iostream>
#include<string>
#include<cstdlib>
using namespace std;
struct node {
   int data;
   node *next;
};

class linked_list {
   private:
      node * head;
   public:
      void add_to_tail(int data);
      void add_to_head(int data);
      void reverse_list();
      void reverse_in_groups(int size);
      void print_list();
      linked_list();
      ~linked_list();
};
void linked_list::add_to_tail(int data)
{
  node *temp = new node;
  temp->data = data;
  temp->next = NULL;
  if (head)
    {
      node *temp_head = head;
      while (temp_head->next)
	temp_head = temp_head->next;
      temp_head->next = temp;
    }
  else
    head = temp;
}
void linked_list::add_to_head(int data)
{
  node *temp = new node;
  temp->data = data;
  temp->next = NULL;
  if (head) {
    temp->next = head;
    head = temp;
  } else
    head = temp;
}
void linked_list::reverse_list()
{
  node *temp = head;
  node *next_temp = NULL;
  node *new_head = NULL;
  while (temp)
    {
      if (new_head)
	{
	  next_temp = temp->next;
          temp->next = new_head;
          new_head = temp;
	  temp = next_temp;
	}
      else
	{
	  new_head = temp;
	  temp = temp->next;
	  new_head->next = NULL;
	}
    }
  head = new_head;
}
void linked_list::reverse_in_groups(int size)
{
  node *temp = head;
  node *next_temp = NULL;
  node *new_head = NULL;
  node *first_list = NULL;
  int temp_size = size;
  node *first_lists_tail = NULL;
  node *temp_first_lists_tail = NULL;
  while (temp)
    {
      if (new_head)
	{
	  next_temp = temp->next;
	  temp->next = new_head;
	  new_head = temp;
	  temp = next_temp;
	}
      else
	{
	  first_lists_tail = temp;
	  new_head = temp;
	  temp = temp->next;
	  new_head->next = NULL;
	}
      temp_size--;
      if (temp_size == 0 || temp == NULL)
	{
	  if (first_list)
	    {
	      temp_first_lists_tail->next = new_head;
	    }
	  else
	    first_list = new_head;
          temp_first_lists_tail = first_lists_tail;
	  new_head = NULL;
	  temp_size = size;
	}
    }
  head = first_list;
}
void linked_list::print_list()
{
  node *temp = head;
  while (temp)
    {
      if (temp->next)
	cout << temp->data << "->";
      else
	cout << temp->data<<"->NULL"<< endl;
      temp = temp->next;
    }
}
linked_list::linked_list()
{
   head = NULL;
}
linked_list::~linked_list()
{
  node *temp;
  while(head)
  {
    temp = head;
    head = head->next;
    delete temp;
  }
}
int main(int argc, char *argv[])
{
  linked_list ll;
  string str;
  string strtmp;
  int pos = 0;
  int loc = 0;
  cin >> str;

  do
    {

      loc = str.find (',', pos);
      strtmp = str.substr (pos, loc - pos);
      ll.add_to_tail ((int) atoi (strtmp.c_str ()));
      pos = loc + 1;
    }
  while (loc > 0);
  ll.print_list ();
  ll.reverse_in_groups (3);
  ll.print_list ();
  return 0;
}

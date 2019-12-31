#include "List.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;

//class list functions
List::List()
: head(NULL), size(0)
{
  cout << "I just created a list!\n";
}

List::~List()
{
  delete head;
  cout << "I just destroyed a list!\n";
}

int List::getSize()
{
  return size;
}

void List::addHeadNode(const int & row)
{
  HeadNode * temp = head;
  if(size == 0)//this is the first node to add
  {
    temp = new HeadNode(col, elem);
  }
  else
  {
    while(temp->down != NULL)
    {
      temp = temp->down;
    }
    temp->down = new HeadNode(col, elem);
  }
  size ++;
}
//class headone functions

HeadNode::HeadNode(int siz, int ro)
: down(NULL), next(NULL), size(siz), row(ro)
{
  cout << "I just created a HeadNode!\n";
}

HeadNode::~HeadNode()
{
  if(next != NULL)
    delete next;
  if(down != NULL)
    delete down;
  cout << "I just destroyed a HeadNode!\n";
}

void HeadNode::addNode(const int & col, const int & elem)
{
  Node * temp = next;
  if(size == 0)//this is the first node to add
  {
    temp = new Node(col, elem);
  }
  else
  {
    while(temp->next != NULL)
    {
      temp = temp->next;
    }
    temp->next = new Node(col, elem);
  }
  size ++;
}
//class node functions

Node::Node(const int & col, const int & elem)
: column(col), element(elem)
{
  cout << "I just created a node!\n";
}

Node::~Node()
{
  cout << "I just destroyed a node!\n";
  if(next != NULL)
    delete next;
}


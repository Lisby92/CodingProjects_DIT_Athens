#ifndef __LISTHPP__
#define __LISTHPP__

class Node
{
public:
  Node(int, int);
 ~Node();
  Node * next;
  int getCol();
  void setCol();
  int getElement();
  void setElement();
private:  

  int column;
  int element;  

};

class HeadNode
{
public:
  HeadNode(int, int);
  ~HeadNode();
  Node * next;
  HeadNode * down;
  void addNode(const int &, const int &);
  int getSize();
private:
  int size;
  int row;

};

class List
{
public:
  //constructor
  List();
  //destructor
  ~List();
  /* MUTATORS */
  int getSize();
  HeadNode * head;
  void addHeadNode(const int &, const int &); // addsaa new headnode
private:
  // data
  int size;
};
#endif
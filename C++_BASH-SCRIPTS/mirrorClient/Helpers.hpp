#ifndef __HELPERS__
#define __HELPERS__
#include <string>
using std::string;

//helper classes to handle new and leaving clients
bool isNumber(string str);//checks if given string is a number
//helper classes to store processed file paths
class Node
{
public:
  Node(string);//constructor
  ~Node();//destructor
  Node * next;//points to next node
  string getPath() const; //get the path to the file

private:
  string path;//path to the file
};

class List
{
public:
  List(int);//constructor
  ~List();//destructor
  void addNode(string);//adds and links a new node
  int getID() const; //get client owner id
  bool searchFiles(string) const;//search for newly added clients
  string extractID(string) const;//extract client id from the path
  void delNode(string);//remove a node
private:
  Node * head;//first node of the list
  int size;//total nodes
  int clientID;//owner client of list
};

class Node1
{
  public:
  Node1(string);//constructor
  ~Node1();//destructor
  Node1 * next;//points to next node
  string getPath() const;//get path to the file
  bool getFound() const;//get a file status
  void setFound(const bool &);//update the status

private:
  string path;//path to the file
  bool found;//true = path found, false = client left
};

class List1
{
public:
  List1(int);//constructor
  ~List1();//destructor
  void addNode(string);//adds and links a new node
  int getID() const;//get owner id
  void checkFiles(string);//look for the target path in the list, and mark it
  void resetFound();//reset for next search
  void delNode(string);//remove a node
  string checkDeleted(int &) const;//check for left clients

private:
  Node1 * head;//first node of the list
  int size;//total nodes
  int clientID;//owner client of list
};

#endif
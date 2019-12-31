#ifndef __HELPERSSERVERHPP__
#define __HELPERSSERVERHPP__
#include <string>
#include <iostream>
using std::string;

bool isNumber(const string &);//checks if given string is a positive number

class Node
{
public:
  Node(string, int);//constructor
  ~Node();//destructor
  Node * next;//points to next node
  string getIP() const; //get the IP
  int getPort() const; //get the port

private:
  string clientIP;//client's IP address
  int clientPortNum;//client's port number
};

class List
{
public:
  List();//constructor
  ~List();//destructor
  bool addNode(string, const int &);//adds and links a new node
  void delNode(string, const int &);//remove a node
  int getSize() const;//get list's current size
  string getClients(string, const int &);//get client list for GET_CLIENTS msg
  string getClients2(string, const int &);//get client list for USER_OFF msg
  bool findClient(string, const int &) const;//check's if target client exists

private:
  Node * head;//first node of the list
  int size;//total nodes
};

#endif
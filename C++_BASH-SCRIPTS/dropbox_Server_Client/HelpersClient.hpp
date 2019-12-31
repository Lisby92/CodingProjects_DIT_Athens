#ifndef __HELPERSCLIENTHPP__
#define __HELPERCLIENTSHPP__
#include <string>
#include <iostream>
#include <cstring>
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
  int getSize() const;//get list size
  string getClients(string, const int &);//get connected clients list
  bool findClient(string, const int &) const;//check if a client is currently on

private:
  Node * head;//first node of the list
  int size;//total nodes
};

class Item
{
public:
  Item(string, int, string, int);//constructor
  ~Item();//destructor
  string getPath() const;//get file/dir path
  int getVersion() const;//get file/dir version
  string getIP() const;//get remote client's IP
  int getPort() const;//get remote client's port
  void setPath(string newPath);//set a new file/dir path
  void setVersion(const int & newVers);//set a new file/dir version
  void setIP(string newIP);//set a new remote client's IP
  void setPort(const int & newPort);//set a new remote client's port
  void setClear();//set item values to default/invalid
  
private:
  string pathname;
  int version;
  string addrIP;
  int portNum;
};

class CyBuffer//circular queue array implementation
{
public:
  CyBuffer(int);//constructor
  ~CyBuffer();//destructor
  bool addItem(string, const int &, string, const int &);//add and link a new item
  Item * getItem() const;//return front item
  bool removeItem();//remove front item
  int getCurItems() const;//get number of total items in the buffer

private:
  Item ** itemsArray;
  int size;//max capacity
  int rear;//points at the last item
  int front;//points at the first item
  int curItems;//current capacity
};

class Node2
{
public:
  Node2(string, int);//constructor
  ~Node2();//destructor
  string getPath() const;//get file/dir path
  int getVersion() const;//get file/dir version
  void setVersion(const int &);//update file/dir version
  Node2 * next;//points to the next node2

private:
  string path;
  int version;
};

class VersionList
{
public:
  VersionList();//constructor
  ~VersionList();//destructor
  int getSize() const;//get current list size
  void addNode(string, const int &);//add and link a new node2
  void delNode(string, const int &);//find and remove a target node2
  bool findNode(string, const int &);//find a target node2
  int getVersion(string);//get a target node2's version
  
private:
  Node2 * head;//head of the list
  int size;//current size
};

long hashFunction(string, string);
//returns a hash of given path and/or containing data in case of file

#endif
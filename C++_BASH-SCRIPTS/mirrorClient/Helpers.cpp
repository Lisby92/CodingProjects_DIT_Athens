#include "Helpers.hpp"
#include <iostream>
using std::cout;
using std::endl;

bool isNumber(string str)
{ //this function checks if str is a non negative integer
  if(!(str[0] >= '0' && str[0] <= '9'))
    return false;
  for(int i = 1; str[i] != '\0'; i++)
    if(str[i] < '0' || str[i] > '9')
      return false;
  return true;//str is number
}

Node::Node(string newPath)
: path(newPath)
{
  //cout << "Node : " << path << " constructed successfully!\n";
}

Node::~Node()
{
  //cout << "Node : " << path << " destroyed successfully!\n";
}

string Node::getPath() const
{
  return path;
}

List::List(int newID)
: head(NULL), size(0), clientID(newID)
{
  //cout << "List for client " << clientID << " constructed successfully!\n";
}

List::~List()
{
  Node * cur = head;
  Node * temp = NULL;
  while(cur != NULL)
  {
    temp = cur;
    cur = cur->next;
    delete temp;
  }
  //cout << "List for client " << clientID << " destroyed successfully!\n";
}

void List::addNode(string newPath)
{
  Node * node = new Node(newPath);
  node->next = head;
  head = node;
  size++;
}

int List::getID() const
{
  return clientID;
}

bool List::searchFiles(string newPath) const
{
  Node * cur = head;
  string curPath;
  bool result = false;
  int lastSlashPos = newPath.find_last_of("/");
  string clID = newPath.substr(lastSlashPos+1);
  int dotPos = clID.find_last_of(".");
  clID = clID.substr(0, dotPos);
  if(stoi(clID, nullptr, 10) == clientID)//skip
    return true;
  while(cur != NULL)
  {
    curPath = cur->getPath();
    if(curPath.compare(newPath) == 0)//already processed
    {
      result = true;
      break;
    }
    cur = cur->next;
  }
  return result;
}

string List::extractID(string target) const
{
  Node * cur = head;
  while(cur != NULL)
  {
    if(target.compare(cur->getPath()) == 0)//found it
    {
      int lastSlashPos = target.find_last_of("/");
      string clID = target.substr(lastSlashPos+1);
      int dotPos = clID.find_last_of(".");
      clID = clID.substr(0, dotPos);
      return clID;
    }
    cur = cur->next;
  }
  return NULL;//error
}

void List::delNode(string target)
{
  string curPath = head->getPath();
  if(curPath.compare(target) == 0)//found at head
  {
    Node * temp = head;
    head = head->next;
    temp->next = NULL;
    delete temp;
    size--;
  }
  else
  {
    Node * temp = head;
    Node * cur = head;
    while(curPath.compare(target) != 0)
    {
      temp = cur;
      cur = cur->next;
      curPath = cur->getPath();
    }
    temp->next = cur->next;
    delete cur;
    size--;
  }

}

Node1::Node1(string newPath)
: path(newPath), found(true)
{
  //cout << "Node1 : " << path << ", found : " << found << " constructed successfully!\n";
}

Node1::~Node1()
{
  //cout << "Node1 : " << path << ", found : " << found << " destroyed successfully!\n";
}

string Node1::getPath() const
{
  return path;
}

bool Node1::getFound() const
{
  return found;
}

void Node1::setFound(const bool & newFound)
{
  found = newFound;
  //cout << path << " set to " << found << endl;
}

List1::List1(int newID)
: head(NULL), size(0), clientID(newID)
{
  //cout << "List for client " << clientID << " constructed successfully!\n";
}

List1::~List1()
{
  Node1 * cur = head;
  Node1 * temp = NULL;
  while(cur != NULL)
  {
    temp = cur;
    cur = cur->next;
    delete temp;
  }
  //cout << "List1 for client " << clientID << " destroyed successfully!\n";
}

void List1::addNode(string newPath)
{
  Node1 * node = new Node1(newPath);
  node->next = head;
  head = node;
  size++;
}

int List1::getID() const
{
  return clientID;
}

void List1::checkFiles(string newPath)
{
  Node1 * cur = head;
  string curPath;
  int lastSlashPos = newPath.find_last_of("/");
  string clID = newPath.substr(lastSlashPos+1);
  int dotPos = clID.find_last_of(".");
  clID = clID.substr(0, dotPos);
  if(stoi(clID, nullptr, 10) == clientID)//skip same id with client
    return;
  while(cur != NULL)
  {
    curPath = cur->getPath();
    if(curPath.compare(newPath) == 0)//.id file found make it true
    {
      cur->setFound(true);
      break;
    }
    cur = cur->next;
  }
}

void List1::resetFound()
{
  Node1 * cur = head;
  string curPath;
  while(cur != NULL)
  {
    curPath = cur->getPath();
    int lastSlashPos = curPath.find_last_of("/");
    string clID = curPath.substr(lastSlashPos+1);
    int dotPos = clID.find_last_of(".");
    clID = clID.substr(0, dotPos);
    if(stoi(clID, nullptr, 10) == clientID)//skip same id with client
      cur->setFound(true);
    else
      cur->setFound(false);
    cur = cur->next;
  }
}

void List1::delNode(string target)
{
  string curPath = head->getPath();
  if(curPath.compare(target) == 0)//found at head
  {
    Node1 * temp = head;
    head = head->next;
    temp->next = NULL;
    delete temp;
    size--;
  }
  else
  {
    Node1 * temp = head;
    Node1 * cur = head;
    while(curPath.compare(target) != 0)
    {
      temp = cur;
      cur = cur->next;
      curPath = cur->getPath();
    }
    temp->next = cur->next;
    delete cur;
    size--;
  }

}

string List1::checkDeleted(int & total) const
{
  string result = "";
  Node1 * cur = head;
  while(cur != NULL)
  {
    if(cur->getFound() == false)
    {
      total++;
      result += cur->getPath();
      result += "*";
    }
    cur = cur->next;
  }
  return result;
}

#include "HelpersServer.hpp"
using std::string;
using std::isdigit;
using std::cout;
using std::to_string;

bool isNumber(const string &s)
{
  string::const_iterator it = s.begin();
  while(it != s.end() && isdigit(*it))
    ++it;
  return (!s.empty() && it == s.end());
}

Node::Node(string newIP, int newPort)
: next(NULL), clientIP(newIP), clientPortNum(newPort)
{
  //cout << "Node : " << clientIP << " ";
  //cout << clientPortNum << " constructed successfully!\n";
}

Node::~Node()
{
  //cout << "Node : " << clientIP << " ";
  //cout << clientPortNum << " destroyed successfully!\n";
}

string Node::getIP() const
{
  return clientIP;
}

int Node::getPort() const
{
  return clientPortNum;
}

List::List()
: head(NULL), size(0)
{
  //cout << "List for server clients constructed successfully!\n";
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
  //cout << "List for server clients destroyed successfully!\n";
}
/********************************NOTE******************/
bool List::addNode(string newIP, const int & newPort)
{
  Node * temp = head;
  bool result = true;
  while(temp != NULL)
  {
    int curPort = temp->getPort();
    string curIP = temp->getIP();
    if((curPort == newPort) && (newIP.compare(curIP) == 0))// correct check
    //if(curPort == newPort)//loopback 127.0.0.1 testing only
    {
      result = false;
      break;
    }
    temp = temp->next;
  }
  if(result == true)
  {
    Node * node = new Node(newIP, newPort);
    node->next = head;
    head = node;
    size++;
  }
  return result;
}

string List::getClients(string askIP, const int & askPort)
{
  Node * temp = head;
  if(head->next == NULL)//list with just the first client after log_on inside
    return "";
  string creation = "CLIENT_LIST";
  creation += " ";
  creation += to_string(getSize()-1);//exclude self
  creation += " ";
  while(temp != NULL)
  {
    int curPort = temp->getPort();
    string curIP = temp->getIP();
    if(curPort != askPort || curIP.compare(askIP) != 0)
    //if(curPort != askPort)//loopback 127.0.0.1 testing only
    {
      creation += curIP;
      creation += " ";
      creation += to_string(curPort);
      creation += " ";
    }
    temp = temp->next;
  }
  return creation;
}

string List::getClients2(string askIP, const int & askPort)
{
  Node * temp = head;
  if(head == NULL)//list with just the first client after log_on inside
    return "";
  string creation = "CLIENT_LIST";
  creation += " ";
  while(temp != NULL)
  {
    int curPort = temp->getPort();
    string curIP = temp->getIP();
    if(curPort != askPort || curIP.compare(askIP) != 0)
    //if(curPort != askPort)//loopback 127.0.0.1 testing only
    {
      creation += curIP;
      creation += " ";
      creation += to_string(curPort);
      creation += " ";
    }
    temp = temp->next;
  }
  return creation;
}

bool List::findClient(string askIP, const int & askPort) const
{
  Node * temp = head;
  bool result = false;
  while(temp != NULL)
  {
    int curPort = temp->getPort();
    string curIP = temp->getIP();
    if(curPort == askPort && askIP.compare(curIP) == 0)//found him
    //if(curPort == askPort)//loopback 127.0.0.1 testing only
    {
      result = true;
      break;
    }
    temp = temp->next;
  }
  return result;
}

int List::getSize() const
{
  return size;
}

void List::delNode(string targetIP, const int & targetPort)
{
  if(head == NULL)
    return;
  string curIP = head->getIP();
  int curPort = head->getPort();
  if(curIP.compare(targetIP) == 0 && targetPort == curPort)//found at head
  //if(targetPort == curPort)//loopback 127.0.0.1 testing only
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
    while(curIP.compare(targetIP) != 0 || curPort != targetPort)
    //while(curPort != targetPort) //loopback 127.0.0.1 testing only
    {
      temp = cur;
      cur = cur->next;
      curIP = cur->getIP();
      curPort = cur->getPort();
    }
    temp->next = cur->next;
    delete cur;
    size--;
  }
}
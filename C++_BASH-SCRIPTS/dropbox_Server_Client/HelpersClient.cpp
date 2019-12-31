#include "HelpersClient.hpp"
using std::string;
using std::isdigit;
using std::cout;
using std::to_string;
using std::endl;

bool isNumber(const string &s)
{
    string::const_iterator it = s.begin();
    while(it != s.end() && isdigit(*it))
      ++it;
    return !s.empty() && it == s.end();
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
    if((curPort == newPort) && (newIP.compare(curIP) == 0))
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
    int counter = size;
    while(curIP.compare(targetIP) != 0 && curPort != targetPort)
    //while(curPort != targetPort) //loopback 127.0.0.1 testing only
    {
      counter--;
      if(counter == 1)
        return;
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

bool List::findClient(string askIP, const int & askPort) const
{
  Node * temp = head;
  bool result = false;
  while(temp != NULL)
  {
    int curPort = temp->getPort();
    string curIP = temp->getIP();
    if(curPort == askPort && askIP.compare(curIP) == 0)
    //if(curPort == askPort)//loopback 127.0.0.1 testing only
    {
      result = true;
      break;
    }
    temp = temp->next;
  }
  return result;
}

Item::Item(string path, int vers, string addr, int port)
: pathname(path), version(vers), addrIP(addr), portNum(port)
{
  /*
  cout << "Item created with the following attributes\n";
  cout << "Pathname : " << pathname << endl;
  cout << "Version : " << version << endl;
  cout << "IP address : " << addrIP << endl;
  cout << "Port number : " << portNum << endl;
  */
}

Item::~Item()
{
  /*
  cout << "Item to be destroyed with the following attributes\n";
  cout << "Pathname : " << pathname << endl;
  cout << "Version : " << version << endl;
  cout << "IP address : " << addrIP << endl;
  cout << "Port number : " << portNum << endl;
  */
}

string Item::getPath() const
{
  return pathname;
}

int Item::getVersion() const
{
  return version;
}

string Item::getIP() const
{
  return addrIP;
}

int Item::getPort() const
{
  return portNum;
}

void Item::setPath(string newPath)
{
  pathname = newPath;
}

void Item::setVersion(const int & newVers)
{
  version = newVers;
}

void Item::setIP(string newIP)
{
  addrIP = newIP;
}

void Item::setPort(const int & newPort)
{
  portNum = newPort;
}

void Item::setClear()
{
  pathname = "";
  version = -2;
  addrIP = "";
  portNum = -1;
}

CyBuffer::CyBuffer(int sz)
: size(sz), rear(-1), front(-1), curItems(0)
{
  //cout << "Cyclic buffer constructed successfully!\n";
  itemsArray = new Item*[size];
  for(int i = 0; i < size; i++)//init empty spots
    itemsArray[i] = new Item("", -2, "", -1);
}

CyBuffer::~CyBuffer()
{
  for(int i = 0; i < size; i++)
    delete itemsArray[i];
  delete[] itemsArray;
  //cout << "Cyclic buffer destroyed successfully!\n";
}

bool CyBuffer::addItem(string path, const int & vers, string addr, const int & port)
{
  if((front == 0 && rear == size-1) || (rear == (front-1)%(size-1))) 
  { 
    //cout << "Buffer is full!\n"; 
    return false; 
  } 
  else if(front == -1) /* Insert First Element */
  { 
    front = rear = 0; 
    itemsArray[rear]->setPath(path);
    itemsArray[rear]->setVersion(vers);
    itemsArray[rear]->setIP(addr);
    itemsArray[rear]->setPort(port);
  }
  else if((rear == size-1) && (front != 0)) 
  { 
    rear = 0; 
    itemsArray[rear]->setPath(path);
    itemsArray[rear]->setVersion(vers);
    itemsArray[rear]->setIP(addr);
    itemsArray[rear]->setPort(port);
  } 
  else
  { 
    rear++; 
    itemsArray[rear]->setPath(path);
    itemsArray[rear]->setVersion(vers);
    itemsArray[rear]->setIP(addr);
    itemsArray[rear]->setPort(port);
  }
  //cout << "Item added with the following attributes\n";
  //cout << "Pathname : " << path << endl;
  //cout << "Version : " << vers << endl;
  //cout << "IP address : " << addr << endl;
  //cout << "Port number : " << port << endl;
  curItems++;
  return true;
}

Item * CyBuffer::getItem() const
{
  return itemsArray[front];
}

bool CyBuffer::removeItem() 
{ 
  if(front == -1)//buffer is empty
    return false;
  itemsArray[front]->setClear();//initialise item
  if(front == rear) 
  { 
    front = -1; 
    rear = -1; 
  } 
  else if(front == size-1) 
    front = 0; 
  else
    front++;
  curItems--;
  return true;
}

int CyBuffer::getCurItems() const
{
  return curItems;
}

Node2::Node2(string newPath, int newVers)
: next(NULL), path(newPath), version(newVers)
{
  //cout << "Node2 constructed successfully!\n";
  //cout << "Pathname : " << path << endl;
  //cout << "Version : " << version << endl;
}

Node2::~Node2()
{
  //cout << "Node2 destroyed successfully!\n";
  //cout << "Pathname : " << path << endl;
  //cout << "Version : " << version << endl;
}

string Node2::getPath() const
{
  return path;
}

int Node2::getVersion() const
{
  return version;
}

void Node2::setVersion(const int & newVers)
{
  version = newVers;
}

VersionList::VersionList()
: head(NULL), size(0)
{
  //cout << "VersionList constructed successfully!\n";
}

VersionList::~VersionList()
{
  Node2 * cur = head;
  Node2 * temp = NULL;
  while(cur != NULL)
  {
    temp = cur;
    cur = cur->next;
    delete temp;
  }
  //cout << "VersionList destroyed successfully!\n";
}

int VersionList::getSize() const
{
  return size;
}

void VersionList::addNode(string path, const int & version)
{
  //cout << "adding node : " << path << " , " << version << endl;
  Node2 * temp = new Node2(path, version);
  temp->next = head;
  head = temp;
  size++;
}

int VersionList::getVersion(string targetPath)
{
  Node2 * cur = head;
  int curVersion = -1;
  while(cur != NULL)
  {
    string curPath = cur->getPath();
    if(curPath.compare(targetPath) == 0)
    {
      curVersion = cur->getVersion();
      break;
    }
    cur = cur->next;
  }
  return curVersion;
}

void VersionList::delNode(string targetPath, const int & targetVersion)
{
  //cout << "deleting node : " << targetPath << ", " << targetVersion << endl;
  if(head == NULL)
    return;
  string curPath = head->getPath();
  int curVersion = head->getVersion();
  if(targetVersion == curVersion && targetPath.compare(curPath) == 0)
  {
    Node2 * temp = head;
    head = head->next;
    temp->next = NULL;
    delete temp;
    size--;
  }
  else
  {
    Node2 * temp = head;
    Node2 * cur = head;
    while(targetVersion != curVersion && targetPath.compare(curPath) != 0) 
    {
      temp = cur;
      cur = cur->next;
      curPath = cur->getPath();
      curVersion = cur->getVersion();
    }
    temp->next = cur->next;
    delete cur;
    size--;
  }
}

bool VersionList::findNode(string targetPath, const int & targetVersion)
{
  bool result = false;
  if(head == NULL)
    return result;
  Node2 * temp = head;
  while(temp != NULL)
  {
    int curVersion = temp->getVersion();
    string curPath = temp->getPath();
    if(curVersion == targetVersion && curPath.compare(targetPath) == 0)
    {
      result = true;
      break;
    }
    temp = temp->next;
  }
  return result;
}

long hashFunction(string data, string extra)
{
  long length = data.length();
  long sum = 0;
  for(long i = 0; i < length; i++)
    sum += long (data[i]);
  long hash = sum % 5120296366660318073;//a 64 bit prime
  if(hash == 0)
  {
    long length2 = extra.length();
    long sum2 = 0;
    for(long i = 0; i < length2; i++)
      sum2 += long (extra[i]);
    hash = sum2 % 1640527770187974607;//another one
  }
  //cout << "hash calculated : " << hash << endl;
  return hash;
}
#include "Helpers.hpp"
#include <iostream>

using namespace std;

BTCNode::BTCNode(int newID, string newOwner)
: BTCid(newID), owner(newOwner)
{
  //cout << "BTCNode : " << BTCid << ", Owner: ";
  //cout << owner <<" constructed successfully!\n";
}

BTCNode::~BTCNode()
{
  //cout << "BTCNode : " << BTCid << ", Owner: ";
  //cout << owner <<" destructed successfully!\n";
}

void BTCNode::print() const
{
  cout << BTCid << endl;
}

int BTCNode::getID() const
{
  return BTCid;
}

string BTCNode::getOwner() const
{
  return owner;
}

List::List()
: head(NULL), size(0)
{
  //cout << "List constructed successfully!\n";
}

List::~List()
{
  BTCNode * cur = head;
  BTCNode * temp = NULL;
  while(cur != NULL)
  {
    temp = cur;
    cur = cur->next;
    delete temp;
  }
  //cout << "List destructed successfully!\n";
}

void List::addNode(const int & newID, string newOwner)
{
  BTCNode * node = new BTCNode(newID, newOwner);
  node->next = head;
  head = node;
  size++;
}

void List::print() const
{
  BTCNode * cur = head;
  while(cur != NULL)
  {
  	cur->print();
  	cur = cur->next;
  }
}

bool List::searchDuplicate() const
{
  BTCNode * start = head;
  BTCNode * cur = head->next;
  int startID;
  int curID;
  while(start != NULL)
  {
  	startID = start->getID();
  	while(cur != NULL)
  	{
  	  curID = cur->getID();
  	  if(startID == curID)
  	  	return true;
  	  cur = cur->next;
  	}
  	start = start->next;
  	if(start != NULL)
  	  cur = start->next;
  }
  return false;
}

int List::getSize() const
{
  return size;
}

void List::getOwners(string * ownerArray) const
{
  BTCNode * cur = head;
  int pos = 0;
  while(cur != NULL)
  {
    ownerArray[pos] = cur->getOwner();
    pos++;
    cur = cur->next;
  }
}

void List::getIDs(int * idArray) const
{
  BTCNode * cur = head;
  int pos = 0;
  while(cur != NULL)
  {
    idArray[pos] = cur->getID();
    pos++;
    cur = cur->next;
  }
}

bool List::searchID(const int & target) const
{
  BTCNode * cur = head;
  int pos = 0;
  int curID;
  while(cur != NULL)
  {
    curID = cur->getID();
    if(curID == target)//found it
      return true;
    pos++;
    cur = cur->next;
  }
  return false;//not found
}

bool isNumber(string str)
{
  if(!(str[0] >= '0' && str[0] <= '9'))
    return false;
  for(int i = 1; str[i] != '\0'; i++)
    if(str[i] < '0' || str[i] > '9')
      return false;
  return true;//str is number
}

bool dateCheck(const int & year, const int & month, const int & day, 
               const int & hour, const int & min,
               const int & lastYear, const int & lastMonth, const int & lastDay,
               const int & lastHour, const int & lastMin)
{
  if(year > lastYear)
  {
    return true;
  }
  else if(year == lastYear)//check month
  {
    if(month > lastMonth)
    {
      return true;
    }
    else if(month == lastMonth)//check day
    {
      if(day > lastDay)
      {
        return true;
      }
      else if(day == lastDay)//check hour
      {
        if(hour > lastHour)
        {
          return true;
        }
        else if(hour == lastHour)//check minute
        {
          if(min > lastMin)
          {
            return true;
          }
          //else its exactly the same date and time or @ least 1 min earlier
        }
      }
    }
  }
  return false;
}

bool breakDownTx(string & tx, int & txID, string & sender, string & receiver,
                 int & amount, int & day, int & month, int & year,
                 int & hour, int & min)
{
  int length = tx.length();
  int counter = 0;
  int pos1 = 0;
  string temp;
  string txDate;
  string txTime;
  int totalSpaces = 0;
  bool isNum;
  int totalTchars = 0;
  int totalDchars = 0;
  bool getPCTflag = false;
  //cout << "string to break:" << tx << endl;
  for(int i = 0; i < length; i++)
  {
    if(tx[i] == ' ')
      totalSpaces++;
    if(tx[i] == '-')
      totalDchars++;
    if(tx[i] == ':')
      totalTchars++;
  }
  if((totalSpaces != 5) && (totalSpaces != 3))
    return false;//error
  if(totalDchars != 2 && totalTchars != 1)
    getPCTflag = true;
  while(counter < length)
  {
    if(tx[pos1] == ' ')
      break;
    else
      pos1++;
    counter++;
  }
  isNum = isNumber(tx.substr(0,pos1));
  if(isNum == false)
    return false;
  txID = stoi(tx.substr(0,pos1), nullptr, 10);
  if(txID < 0)//
    return false;
  temp = tx.substr(pos1+1);
  //cout << temp << ": is left\n";
  length = temp.length();
  counter = 0;
  pos1 = 0;
  while(counter < length)
  {
    if(temp[pos1] == ' ')
      break;
    else
      pos1++;
    counter++;
  }
  sender = temp.substr(0, pos1);
  //cout << sender << ": is the sender\n";
  temp = temp.substr(pos1+1);
  //cout << temp << ": is left\n";
  length = temp.length();
  counter = 0;
  pos1 = 0;
  while(counter < length)
  {
    if(temp[pos1] == ' ')
      break;
    else
      pos1++;
    counter++;
  }
  receiver = temp.substr(0, pos1);
  //cout << receiver << ": is the receiver\n";
  temp = temp.substr(pos1+1);
  //cout << temp << ": is left\n";
  length = temp.length();
  counter = 0;
  pos1 = 0;
  while(counter < length)
  {
    if(temp[pos1] == ' ')
      break;
    else
      pos1++;
    counter++;
  }
  isNum = isNumber(temp.substr(0, pos1));
  if(isNum == false)
    return false;
  amount = stoi(temp.substr(0, pos1), nullptr, 10);
  if(amount <= 0)//cant be negative or 0
    return false;
  //cout << amount << ": is the amount!\n";
  if(getPCTflag == false)
  {
    temp = temp.substr(pos1+1);
    length = temp.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == ' ')
        break;
      else
        pos1++;
      counter++;
    }
    txDate = temp.substr(0, pos1);
    txTime = temp.substr(pos1+1);
    //cout << txDate << ": is the date\t";
    //cout << txTime << ": is the time\n";
    length = txDate.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(txDate[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(txDate.substr(0, pos1));
    if(isNum == false)
      return false;
    day = stoi(txDate.substr(0, pos1), nullptr, 10);
    if(day > 31 || day < 1)// day 1~31
      return false;
    txDate = txDate.substr(pos1+1);
    length = txDate.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(txDate[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(txDate.substr(0, pos1));
    if(isNum == false)
      return false;
    month = stoi(txDate.substr(0, pos1), nullptr, 10);
    if(month > 12 || month < 1)// month 1~12
      return false;
    txDate = txDate.substr(pos1+1);
    length = txDate.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(txDate[pos1] == ' ')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(txDate.substr(0, pos1));
    if(isNum == false)
      return false;
    year = stoi(txDate.substr(0, pos1), nullptr, 10);
    if(year < 0)// (must be Roger Ver aka BTC jesus ;p)
      return false; // year 0~inf 
    length = txTime.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(txTime[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(txTime.substr(0, pos1));
    if(isNum == false)
      return false;
    hour = stoi(txTime.substr(0, pos1), nullptr, 10);
    if(hour < 0 || hour > 23)// hour 0~23
      return false;
    txTime = txTime.substr(pos1+1);
    length = txTime.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(txTime[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(txTime.substr(0, pos1));
    if(isNum == false)
      return false;
    min = stoi(txTime.substr(0, pos1), nullptr, 10);
    if(min < 0 || min > 59) // minute 0~59
      return false;
  }
  else//get local time and use it instead
  {
    time_t t = time(NULL);
    tm* timePtr = localtime(&t);
    min = timePtr->tm_min;
    hour = timePtr->tm_hour;
    day = timePtr->tm_mday;
    month = timePtr->tm_mon + 1;
    year = timePtr->tm_year + 1900;
  }
  //cout << day << "-" << month << "-" << year << "\n";
  //cout << hour << ":" << min << "\n";
  return true;
}
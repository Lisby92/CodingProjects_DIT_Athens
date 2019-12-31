#include "HashTable.hpp"
#include <iostream>
#include "Helpers.hpp"
using namespace std;

Bucket::Bucket(int sz = 100)
: next(NULL), size(sz), curCap(0)
{
  txArray = new TransactionsList*[size];
  for(int i = 0; i < size; i++)
    txArray[i] = new TransactionsList("N/A");
  walletArray = new string[size];
  //cout << "Bucket of size " << size << " created successfully!\n";
}

int Bucket::searchID(string target)
{
  for(int i = 0; i < size; i++)
    if(target.compare(walletArray[i]) == 0)//found wallet
      return i;
  return -1;//not found
}

int Bucket::getCap() const
{
  return curCap;
}

void Bucket::addTx(string sender, string receiver, const int & txID,
                  const int & amount, const int & day, const int & month,
                  const int & year, const int & hour, const int & min,
                  TreeNode * involvedNode, const int & mode1,
                  const int & mode2, const int & coinInv)
{
  if(mode2 == 0)//add at start bucket is new
  {
    if(mode1 == 1)//sender
    {
      txArray[0]->setWalletID(sender);
      addWalletID(sender, 0);
    }
    else if(mode1 == 2)//receiver
    {
      txArray[0]->setWalletID(receiver);
      addWalletID(receiver, 0);
    }
    txArray[0]->addTx(txID, sender, receiver, amount, day, month, year,
                     hour, min, involvedNode, coinInv);
    curCap++;
  }
  else if(mode2 == 1)//add at first free position
  {
    if(mode1 == 1)//sender
    {
      txArray[curCap]->setWalletID(sender);
      addWalletID(sender, curCap);
    }
    else if(mode1 == 2)//receiver
    {
      txArray[curCap]->setWalletID(receiver);
      addWalletID(receiver, curCap);
    }
    txArray[curCap]->addTx(txID, sender, receiver, amount, day, month, year,
                     hour, min, involvedNode, coinInv);
    curCap++;
  }
  else if(mode2 == 2)//find and link at existing
  {
    int pos;
    if(mode1 == 1)//sender 
      pos = searchID(sender);
    else if(mode1 == 2)//receiver
      pos = searchID(receiver);
    txArray[pos]->addTx(txID, sender, receiver, amount, day, month, year,
                     hour, min, involvedNode, coinInv);
  }
  //cout << "a new tx added!\n";
}

int Bucket::getSize() const
{
  return size;
}

void Bucket::addWalletID(string key, const int & pos)
{
  walletArray[pos] = key;
}

Bucket::~Bucket()
{
  for(int i = 0; i < size; i++)
    delete txArray[i];
  delete[] txArray;
  delete[] walletArray;
  //cout << "Bucket of size " << size << " destroyed successfully!\n";
}

bool Bucket::traceCoinHistory(const int & target) const
{
  bool result = false;
  for(int i = 0; i < curCap; i++)
    if(txArray[i]->traceCoinHistory(target) == true)
      result = true;
  return result;
}

int Bucket::searchBTCTxsInv(const int & BTCId) const
{
  int result = 0;
  for(int i = 0; i < curCap; i++)
    result += txArray[i]->searchBTCTxsInv(BTCId);
  return result;
}

void Bucket::findEarnings(string owner, const int & sDay, const int & sMonth, 
                          const int & sYear, const int & sHour,
                          const int & sMinute, const int & eDay,
                          const int & eMonth, const int & eYear, 
                          const int & eHour, const int & eMinute)
{
  bool result = false;
  for(int i = 0; i < curCap; i++)
  {
    if(owner.compare(txArray[i]->getWalletID()) == 0)
    {
      txArray[i]->findEarnings(owner, sDay, sMonth, sYear, sHour, sMinute, 
                                      eDay, eMonth, eYear, eHour, eMinute);
      result = true;
    }
  }
  if(result == false)
  {
    cout << endl;
    cout << owner << " has not received any BTCs yet!\n";
  }
}

void Bucket::findPayments(string owner, const int & sDay, const int & sMonth, 
                          const int & sYear, const int & sHour,
                          const int & sMinute, const int & eDay,
                          const int & eMonth, const int & eYear, 
                          const int & eHour, const int & eMinute)
{
  bool result = false;
  for(int i = 0; i < curCap; i++)
  {
    if(owner.compare(txArray[i]->getWalletID()) == 0)
    {
      txArray[i]->findPayments(owner, sDay, sMonth, sYear, sHour, sMinute, 
                                      eDay, eMonth, eYear, eHour, eMinute);
      result = true;
    }
  }
  if(result == false)
  {
    cout << endl;
    cout << owner << " has not sent any BTCs yet!\n";
  }
}

BucketList::BucketList(int newID, int buckSz)
: size(0), id(newID), bucketSize(buckSz), head(NULL) 
{
  //cout << id << " BucketList has been created!\n";
}

BucketList::~BucketList()
{
  Bucket * cur = head;
  Bucket * temp = NULL;
  while(cur != NULL)
  {
    temp = cur;
    cur = cur->next;
    delete temp;
  }
  //cout << id << " BucketList deleted succesfully!\n";
}

void BucketList::addBucket(const int & buckSz)
{
  Bucket * node = new Bucket(buckSz);
  node->next = head;
  head = node;
  size++;
}

void BucketList::addTx(string sender, string receiver, const int & txID,
                      const int & amount, const int & day, const int & month,
                      const int & year, const int & hour, const int & min,
                      TreeNode * involvedNode,
                      const int & mode, const int & coinInv)
{
  int bucketCap;
  Bucket * curBucket = head;
  int buckSize = getBucketSize();
  int search;
  int mode2;
  if(mode == 1)//sender side
    search = searchID(sender);
  else if(mode == 2)//receiver side
    search = searchID(receiver);
  if(search != -1)//found him somewhere
  {
    int result;
    while(curBucket != NULL)
    {
      if(mode == 1)
        result = curBucket->searchID(sender);
      else if(mode == 2)
        result = curBucket->searchID(receiver);
      if(result != -1)//found him
      {
        mode2 = 2;
        curBucket->addTx(sender, receiver, txID, amount, day, month, year, hour,
                         min, involvedNode, mode, mode2, coinInv);
        break;
      }
      curBucket = curBucket->next;
    }
  }
  else if(search == -1)//have to add him either here or at the next buckets that have space
  {
    int result = 0;
    while(curBucket != NULL)
    {
      bucketCap = curBucket->getCap();
      if(bucketCap < buckSize)//can add here
      {
        result = 1;
        break;
      }
      else//check next
        curBucket = curBucket->next;
    }
    if(result == 1)//to be added in found bucket with enough space(temp)
    {
      mode2 = 1;
      curBucket->addTx(sender, receiver, txID, amount, day, month, year, hour,
                       min, involvedNode, mode, mode2, coinInv);
    }
    else if(result == 0)//have to create a new bucket and add there
    {
      mode2 = 0;
      addBucket(buckSize);
      curBucket = head;
      head->addTx(sender, receiver, txID, amount, day, month, year, hour,
                  min, involvedNode, mode, mode2, coinInv);//pos 0
    }
  }
}

int BucketList::searchID(string target)
{
  Bucket * cur = head;
  int result;
  while(cur != NULL)
  {
    result = cur->searchID(target);
    if(result != -1)
      return result;
    else
      cur = cur->next;
  }
  return -1;
}

int BucketList::getBucketSize() const
{
  return bucketSize;
}

bool BucketList::traceCoinHistory(const int & target) const
{
  bool result = false;
  Bucket * cur = head;
  while(cur != NULL)
  {
    if(cur->traceCoinHistory(target) == true)
      result = true;
    cur = cur->next;
  }
  return result;
}

void BucketList::findEarnings(string owner, const int & sDay, 
                              const int & sMonth, const int & sYear,
                              const int & sHour, const int & sMinute,
                              const int & eDay, const int & eMonth,
                              const int & eYear,   const int & eHour,
                              const int & eMinute)
{
  Bucket * cur = head;
  while(cur != NULL)
  {
    cur->findEarnings(owner, sDay, sMonth, sYear, sHour, sMinute, 
                             eDay, eMonth, eYear, eHour, eMinute);
    cur = cur->next;
  }
}

void BucketList::findPayments(string owner, const int & sDay, 
                              const int & sMonth, const int & sYear,
                              const int & sHour, const int & sMinute,
                              const int & eDay, const int & eMonth,
                              const int & eYear,   const int & eHour,
                              const int & eMinute)
{
  Bucket * cur = head;
  while(cur != NULL)
  {
    cur->findPayments(owner, sDay, sMonth, sYear, sHour, sMinute, 
                             eDay, eMonth, eYear, eHour, eMinute);
    cur = cur->next;
  }
}

int BucketList::searchBTCTxsInv(const int & BTCId) const
{
  Bucket * cur = head;
  int result = 0;
  while(cur != NULL)
  {
    result += cur->searchBTCTxsInv(BTCId);
    cur = cur->next;
  }
  return result;
}

HashTable::HashTable(int sz = 4, int buckSz = 100)
: size(sz)
{
  bucketArray = new BucketList*[size];
  for(int i = 0; i < size; i++)
    bucketArray[i] = new BucketList(i, buckSz);
  //cout << "HashTable created successfully!\n";
}

HashTable::~HashTable()
{
  for(int i = 0; i < size; i++)
    delete bucketArray[i];
  delete[] bucketArray;
  //cout << "HashTable destroyed successfully!\n";
}

int HashTable::hashFunction(string key)
{//create an integer from ascii codes then hash it
  int length = key.length();
  int sum = 0;
  for(int i = 0; i < length; i++)
    sum += int (key[i]);
  int hash = sum % size;
  return hash;
}

void HashTable::findAdd(string target1, string target2, const int & txID,
                        const int & amount, const int & day, const int & month,
                        const int & year, const int & hour, const int & min,
                        const int & mode, TreeNode * involvedNode,
                        const int & coinInv)
{
  int hash;
  if(mode == 1)//target 1 = sender target2 = receiver, sendHT
  {
    hash = hashFunction(target1);
    //cout << "calculated hash " << hash << " from wallet " << target1 << endl;
  }
  else if(mode == 2)//target 1 = sender , target2 = receiver, recvHT
  {
    hash = hashFunction(target2);
    //cout << "calculated hash " << hash << " from wallet " << target2 << endl;
  }
  bucketArray[hash]->addTx(target1, target2, txID, amount, day, month, year, 
                           hour, min, involvedNode, mode, coinInv);
}

bool HashTable::traceCoinHistory(const int & target) const
{
  bool result = false;
  for(int i = 0; i < size; i++)
    if(bucketArray[i]->traceCoinHistory(target) == true)//found coin once
      result = true;
  return result;
}

bool HashTable::findEarnings(string owner, string & sDate, string & eDate,
                             string & sTime, string & eTime)
{
  int hash = hashFunction(owner);
  int sDay = -1;
  int sMonth = -1;
  int sYear = -1;
  int sHour = -1;
  int sMinute = -1;
  int eDay = -1;
  int eMonth = -1;
  int eYear = -1;
  int eHour = -1;
  int eMinute = -1;
  string temp;
  int length;
  bool isNum;
  if((sDate.compare("N/A") == 0) && (eDate.compare("N/A") == 0) && 
     (sTime.compare("N/A") == 0) && (eTime.compare("N/A") == 0))
  {
    cout << "getting all the history!\n";
    //case 1 search all history
  }
  else if((sDate.compare("N/A") == 0) && (eDate.compare("N/A") == 0))
  {
    //case 2 search for specific time frame
    temp = sTime; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    int counter = 0;
    int pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sHour = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sMinute = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << sHour << ":" << sMinute << "\n";
    temp = eTime; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eHour = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eMinute = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << eHour << ":" << eMinute << "\n";
  }
  else if((sTime.compare("N/A") == 0) && (eTime.compare("N/A") == 0))
  {
    //case 3 search for specific date frame
    temp = sDate; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    int counter = 0;
    int pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sDay = stoi(temp.substr(0, pos1), nullptr, 10);
    temp = temp.substr(pos1+1);
    length = temp.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sMonth = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sYear = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << sDay << "-" << sMonth << "-" << sYear << "\n";
    temp = eDate;
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eDay = stoi(temp.substr(0, pos1), nullptr, 10);
    temp = temp.substr(pos1+1);
    length = temp.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eMonth = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eYear = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << eDay << "-" << eMonth << "-" << eYear << "\n";
  }
  else
  {
  	//case 4 search for fully specified date and time frame
  	temp = sDate; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    int counter = 0;
    int pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sDay = stoi(temp.substr(0, pos1), nullptr, 10);
    temp = temp.substr(pos1+1);
    length = temp.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sMonth = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sYear = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << sDay << "-" << sMonth << "-" << sYear << "\n";
    temp = eDate;
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eDay = stoi(temp.substr(0, pos1), nullptr, 10);
    temp = temp.substr(pos1+1);
    length = temp.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eMonth = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eYear = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << eDay << "-" << eMonth << "-" << eYear << "\n";
    temp = sTime; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sHour = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sMinute = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << sHour << ":" << sMinute << "\n";
    //now for end
    temp = eTime; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eHour = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eMinute = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << eHour << ":" << eMinute << "\n";
  }
  bucketArray[hash]->findEarnings(owner, sDay, sMonth, sYear, sHour, sMinute, 
                                         eDay, eMonth, eYear, eHour, eMinute);
  return true;
}

bool HashTable::findPayments(string owner, string & sDate, string & eDate,
                             string & sTime, string & eTime)
{
  int hash = hashFunction(owner);
  int sDay = -1;
  int sMonth = -1;
  int sYear = -1;
  int sHour = -1;
  int sMinute = -1;
  int eDay = -1;
  int eMonth = -1;
  int eYear = -1;
  int eHour = -1;
  int eMinute = -1;
  string temp;
  int length;
  bool isNum;
  if((sDate.compare("N/A") == 0) && (eDate.compare("N/A") == 0) && 
     (sTime.compare("N/A") == 0) && (eTime.compare("N/A") == 0))
  {
    cout << "getting all the history!\n";
    //case 1 search all history
  }
  else if((sDate.compare("N/A") == 0) && (eDate.compare("N/A") == 0))
  {
    //case 2 search for specific time frame
    temp = sTime; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    int counter = 0;
    int pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sHour = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sMinute = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << sHour << ":" << sMinute << "\n";
    //now for end
    temp = eTime; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eHour = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eMinute = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << eHour << ":" << eMinute << "\n";
  }
  else if((sTime.compare("N/A") == 0) && (eTime.compare("N/A") == 0))
  {
    //case 3 search for specific date frame
    temp = sDate; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    int counter = 0;
    int pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sDay = stoi(temp.substr(0, pos1), nullptr, 10);
    temp = temp.substr(pos1+1);
    length = temp.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sMonth = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sYear = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << sDay << "-" << sMonth << "-" << sYear << "\n";
    temp = eDate;
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eDay = stoi(temp.substr(0, pos1), nullptr, 10);
    temp = temp.substr(pos1+1);
    length = temp.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eMonth = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eYear = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << eDay << "-" << eMonth << "-" << eYear << "\n";
  }
  else
  {
    //case 4 search for fully specified date and time frame
    temp = sDate; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    int counter = 0;
    int pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sDay = stoi(temp.substr(0, pos1), nullptr, 10);
    temp = temp.substr(pos1+1);
    length = temp.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sMonth = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sYear = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << sDay << "-" << sMonth << "-" << sYear << "\n";
    temp = eDate;
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eDay = stoi(temp.substr(0, pos1), nullptr, 10);
    temp = temp.substr(pos1+1);
    length = temp.length();
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == '-')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eMonth = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eYear = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << eDay << "-" << eMonth << "-" << eYear << "\n";
    temp = sTime; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sHour = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    sMinute = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << sHour << ":" << sMinute << "\n";
    //now for end
    temp = eTime; 
    length = temp.length();
    temp = temp.substr(1, length-2);
    length -= 2;
    //cout << "cut it to:" << temp << endl;
    counter = 0;
    pos1 = 0;
    while(counter < length)
    {
      if(temp[pos1] == ':')
        break;
      else
        pos1++;
      counter++;
    }
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eHour = stoi(temp.substr(0, pos1), nullptr, 10);
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
    isNum = isNumber(temp.substr(0, pos1));
    if(isNum == false)
      return false;
    eMinute = stoi(temp.substr(0, pos1), nullptr, 10);
    //cout << eHour << ":" << eMinute << "\n";
  }
  bucketArray[hash]->findPayments(owner, sDay, sMonth, sYear, sHour, sMinute, 
                                         eDay, eMonth, eYear, eHour, eMinute);
  return true;
}

int HashTable::searchBTCTxsInv(const int & BTCId) const
{
  int result = 0;
  for(int i = 0; i < size; i++)
    result += bucketArray[i]->searchBTCTxsInv(BTCId);
  return result;
}
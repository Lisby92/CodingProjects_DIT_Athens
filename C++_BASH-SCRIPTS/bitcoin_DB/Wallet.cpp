#include "Wallet.hpp"
#include <iostream>

using namespace std;

CoinNode::CoinNode(int id, int val)
: coinID(id), value(val)
{
  //cout << "Node created successfully!\n";
  //cout << "ID : " << coinID << ", Value : " << value << endl;
}

CoinNode::~CoinNode()
{
  //cout << "Node destructed successfully!\n";
}

int CoinNode::getID() const
{
  return coinID;
}

int CoinNode::getVal() const
{
  return value;
}

void CoinNode::setVal(const int & newVal)
{
  //cout << "BTC ID : " << coinID << endl;
  //cout << "old value : " << value << endl;
  value = newVal;
  //cout << "new value : " << value << endl;
}

HODLStack::HODLStack()
: head(NULL), size(0)
{
  //cout << "HODLStack created successfully!\n";
}

HODLStack::~HODLStack()
{
  CoinNode * cur = head;
  CoinNode * temp = NULL;
  while(cur != NULL)
  {
    temp = cur;
    cur = cur->next;
    delete temp;
  }
  //cout << "HODLStack deleted succesfully!\n";
}

void HODLStack::addNode(const int & id, const int & val)
{
  CoinNode * node = new CoinNode(id, val);
  node->next = head;
  head = node;
  size++;
}

void HODLStack::delNode()
{
  int curVal = head->getVal();
  if(curVal == 0)
  {
    CoinNode * temp = head;
    head = head->next;
    temp->next = NULL;
    delete temp;
    size--;
  }
  else
  {
    CoinNode * temp = head;
    CoinNode * cur = head;
    while(curVal != 0)
    {
      temp = cur;
      cur = cur->next;
      curVal = cur->getVal();
    }
    temp->next = cur->next;
    delete cur;
    size--;
  }
}

void HODLStack::getBTCs(int * btcIDs, int *btcValues, const int & amount,
                        int & emptied)
{
  CoinNode * cur = head;
  int pos = 0;
  int val;
  int left = amount;
  while(cur != NULL)
  {
    val = cur->getVal();
    btcIDs[pos] = cur->getID();
    if(val <= left)//coin emptied
    {
      left -= val;
      btcValues[pos] = val;
      cur->setVal(0);//set 0 for deleting later
      emptied++;
    }
    else//coin has more still, amount gathered
    {
      val -= left;
      btcValues[pos] = left;
      left = 0;
      cur->setVal(val);
    }
    if(left == 0)//amount gathered
      break;
    cur = cur->next;
    pos++;
  }
}

Wallet::Wallet(int sz, int curVal, int * idArray, string owner)
: size(sz), walletID(owner)
{
  totalBalance = size * curVal;
  ledger = new HODLStack();
  if(idArray != NULL)
    for(int i = 0; i < size; i++)
      ledger->addNode(idArray[i], curVal);
  //cout << "Wallet constructed successfully!\n";
}

Wallet::~Wallet()
{
  delete ledger;
  //cout << "Wallet destroyed successfully!\n";
}

int Wallet::getBalance() const
{
  return totalBalance;
}

int Wallet::getSize() const
{
  return size;
}

void Wallet::setBalance(const int & newBalance)
{
  //cout << walletID << "'s old balance : " << totalBalance << "\t";
  totalBalance = newBalance;
  //cout << walletID << "'s new balance : " << totalBalance << endl;
}

void Wallet::setSize(const int & newSize)
{
  size = newSize;
}

int Wallet::checkBalance(string sender, const int & amount) const
{
  if(sender.compare(walletID) == 0)//found him
  {
    if(amount <= totalBalance)//has enuf balance to transact
      return 1;
    else//not enuf balance to transact
      return 2;
  }
  return 0;//not found
}

int Wallet::checkRcvr(string receiver) const
{
  if(receiver.compare(walletID) == 0)//found him
    return 1;
  return 0;//not found
}

void Wallet::getBTCs(int * btcIDs, int *btcValues, const int & amount,
                     int & emptied)
{
  //cout << walletID << "'s old balance : " << totalBalance << endl;
  ledger->getBTCs(btcIDs, btcValues, amount, emptied);//get the data required
  totalBalance -= amount;//update balance
  //cout << walletID << "'s new balance : " << totalBalance << endl;
}

void Wallet::addBTCs(int * btcIDs,int * btcValues, const int & amount,
                     const int & sz)
{
  int cur = 0;
  //cout << walletID << "'s old balance : " << totalBalance << endl;
  while(cur < sz)
  {
    ledger->addNode(btcIDs[cur], btcValues[cur]);
    totalBalance += btcValues[cur];
    size++;
    cur++;
  }
  //cout << walletID << "'s new balance : " << totalBalance << endl;
}

void Wallet::delBTCs()
{
  ledger->delNode();
}

string Wallet::getID() const
{
  return walletID;
}
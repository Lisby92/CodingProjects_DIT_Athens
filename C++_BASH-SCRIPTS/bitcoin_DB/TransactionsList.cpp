#include "TransactionsList.hpp"
#include <iostream>
using namespace std;

Transaction::Transaction(int txID = 1, string sID = "Satoshi",
  string rID = "Nakamoto", int val = 1, int d = 1, int m = 1, int y = 2011,
  int h = 1, int min = 1, int coinInv = 1)
: transactionID(txID), senderWalletID(sID), receiverWalletID(rID),
  value(val), coinInvolved(coinInv)
{
  timeFrame = new TimeFrame(d, m, y, h, min);//create the timeframe
  /*
  cout << "Transaction Entry constructed successfully!\n";
  cout << txID << endl;
  cout << senderWalletID << endl;
  cout << receiverWalletID << endl;
  cout << val << endl;
  cout << coinInvolved << endl;
  */
}

Transaction::~Transaction()
{
  delete timeFrame;
  //cout << "Transaction Entry destroyed successfully!\n";
}

bool Transaction::traceCoinHistory(const int & target, int * txIDs, const int &sz) const
{
  bool result = false;
  if(coinInvolved == target)//found it
  {
    for(int i = 0; i < sz; i++)
      if(txIDs[i] == transactionID)
        return false;
    result = true;
    cout << transactionID << " ";
    cout << senderWalletID << " ";
    cout << receiverWalletID << " ";
    cout << value << " ";
    timeFrame->print();
    for(int i =0; i < sz; i++)
    {
      if(txIDs[i] == -1)
      {
        txIDs[i] = transactionID;
        break;
      }
    }
  }
  return result;
}

int Transaction::searchBTCTxsInv(const int & BTCId, int * txIDs, const int & sz) const
{
  int result = 0;
  if(coinInvolved == BTCId)
  {
    for(int i = 0; i < sz; i++)
      if(txIDs[i] == transactionID)
        return result;
    result++;
    for(int i =0; i < sz; i++)
    {
      if(txIDs[i] == -1)
      {
        txIDs[i] = transactionID;
        break;
      }
    } 
  }
  return result;
}

int Transaction::findEarnings(string owner, const int & sDay, 
                              const int & sMonth, const int & sYear, 
                              const int & sHour, const int & sMinute,
                              const int & eDay, const int & eMonth, 
                              const int & eYear, const int & eHour,
                              const int & eMinute, int & lastTxID) const
{
  int result = 0;
  int resultOut = 0;
  int txDay = timeFrame->getDay();
  int txMonth = timeFrame->getMonth();
  int txYear = timeFrame->getYear();
  int txHour = timeFrame->getHour();
  int txMinute = timeFrame->getMinute();
  int sTotalDays = 0;
  int eTotalDays = 0;
  int cTotalDays = txYear*365 + txMonth*30 + txDay;//transform to days
  int sTotalMinutes = 0;
  int eTotalMinutes = 0;
  int cTotalMinutes = txHour*60 + txMinute;//transform to minutes
  //cout << "days created from current:" << cTotalDays << endl;
  //cout << "minutes created from current:" << cTotalMinutes << endl;
  if(lastTxID == transactionID)
    return result;
  else
    lastTxID = transactionID;
  if(sDay == -1 && sHour == -1)//all history
    resultOut = 1;
  else if(sDay == -1 && sHour != -1)//specific time frame
  {
    sTotalMinutes = sHour*60 + sMinute;
    eTotalMinutes = eHour*60 + eMinute;
    //cout << "minutes created from start:" << sTotalMinutes << endl;
    //cout << "minutes created from end:" << eTotalMinutes << endl;
    if(sTotalMinutes <= cTotalMinutes && cTotalMinutes <= eTotalMinutes)
      resultOut = 1;
  }
  else if(sDay != -1 && sHour == -1)//specific date frame
  {
    sTotalDays = sYear*365 + sMonth*30 + sDay;
    eTotalDays = eYear*365 + eMonth*30 + eDay;
    //cout << "days created from start:" << sTotalDays << endl;
    //cout << "days created from end:" << eTotalDays << endl;
    if(sTotalDays <= cTotalDays && cTotalDays <= eTotalDays)
      resultOut = 1;
  }
  else if(sDay != -1 && sHour != -1)//full date and time frame specified
  {
    sTotalDays = sYear*365 + sMonth*30 + sDay;
    eTotalDays = eYear*365 + eMonth*30 + eDay;
    //cout << "days created from start:" << sTotalDays << endl;
    //cout << "days created from end:" << eTotalDays << endl;
    sTotalMinutes = sHour*60 + sMinute;
    eTotalMinutes = eHour*60 + eMinute;
    //cout << "minutes created from start:" << sTotalMinutes << endl;
    //cout << "minutes created from end:" << eTotalMinutes << endl;
    if(sTotalDays < cTotalDays && cTotalDays < eTotalDays)//dont check time
      resultOut = 1;
    else if(sTotalDays == cTotalDays && eTotalDays > cTotalDays)//check start t
    {
      if(sTotalMinutes <= cTotalMinutes)
        resultOut = 1;
    }
    else if(eTotalDays == cTotalDays && sTotalDays < cTotalDays)//check end time
    {
      if(cTotalMinutes <= eTotalMinutes)
        resultOut = 1;
    }
    else if(eTotalDays == cTotalDays && cTotalDays == sTotalDays)//check both
    {
      if(sTotalMinutes <= cTotalMinutes && cTotalMinutes <= eTotalMinutes)
        resultOut = 1;
    }
  }
  if(resultOut == 1)
  {
    cout << transactionID << " ";
    cout << senderWalletID << " ";
    cout << receiverWalletID << " ";
    cout << value << " ";
    timeFrame->print();
    result = value;
  }
  return result;
}

int Transaction::findPayments(string owner, const int & sDay, 
                              const int & sMonth, const int & sYear, 
                              const int & sHour, const int & sMinute,
                              const int & eDay, const int & eMonth, 
                              const int & eYear, const int & eHour,
                              const int & eMinute, int & lastTxID) const
{
  int result = 0;
  int resultOut = 0;
  int txDay = timeFrame->getDay();
  int txMonth = timeFrame->getMonth();
  int txYear = timeFrame->getYear();
  int txHour = timeFrame->getHour();
  int txMinute = timeFrame->getMinute();
  int sTotalDays = 0;
  int eTotalDays = 0;
  int cTotalDays = txYear*365 + txMonth*30 + txDay;//transform to days
  int sTotalMinutes = 0;
  int eTotalMinutes = 0;
  int cTotalMinutes = txHour*60 + txMinute;//transform to minutes
  //cout << "days created from current:" << cTotalDays << endl;
  //cout << "minutes created from current:" << cTotalMinutes << endl;
  if(lastTxID == transactionID)
    return result;
  else
    lastTxID = transactionID;
  if(sDay == -1 && sHour == -1)//all history
    resultOut = 1;
  else if(sDay == -1 && sHour != -1)//specific time frame
  {
    sTotalMinutes = sHour*60 + sMinute;
    eTotalMinutes = eHour*60 + eMinute;
    //cout << "minutes created from start:" << sTotalMinutes << endl;
    //cout << "minutes created from end:" << eTotalMinutes << endl;
    if(sTotalMinutes <= cTotalMinutes && cTotalMinutes <= eTotalMinutes)
      resultOut = 1;
  }
  else if(sDay != -1 && sHour == -1)//specific date frame
  {
    sTotalDays = sYear*365 + sMonth*30 + sDay;
    eTotalDays = eYear*365 + eMonth*30 + eDay;
    //cout << "days created from start:" << sTotalDays << endl;
    //cout << "days created from end:" << eTotalDays << endl;
    if(sTotalDays <= cTotalDays && cTotalDays <= eTotalDays)
      resultOut = 1;
  }
  else if(sDay != -1 && sHour != -1)//full date and time frame specified
  {
    sTotalDays = sYear*365 + sMonth*30 + sDay;
    eTotalDays = eYear*365 + eMonth*30 + eDay;
    //cout << "days created from start:" << sTotalDays << endl;
    //cout << "days created from end:" << eTotalDays << endl;
    sTotalMinutes = sHour*60 + sMinute;
    eTotalMinutes = eHour*60 + eMinute;
    //cout << "minutes created from start:" << sTotalMinutes << endl;
    //cout << "minutes created from end:" << eTotalMinutes << endl;
    if(sTotalDays < cTotalDays && cTotalDays < eTotalDays)//dont check time
      resultOut = 1;
    else if(sTotalDays == cTotalDays && eTotalDays > cTotalDays)//check start t
    {
      if(sTotalMinutes <= cTotalMinutes)
        resultOut = 1;
    }
    else if(eTotalDays == cTotalDays && sTotalDays < cTotalDays)//check end time
    {
      if(cTotalMinutes <= eTotalMinutes)
        resultOut = 1;
    }
    else if(eTotalDays == cTotalDays && cTotalDays == sTotalDays)//check both
    {
      if(sTotalMinutes <= cTotalMinutes && cTotalMinutes <= eTotalMinutes)
        resultOut = 1;
    }
  }
  if(resultOut == 1)
  {
    cout << transactionID << " ";
    cout << senderWalletID << " ";
    cout << receiverWalletID << " ";
    cout << value << " ";
    timeFrame->print();
    result = value;
  }
  return result;
}

int Transaction::getValue() const
{
  return value;
}

Node::Node(int txID = 1, string sID = "Satoshi", string rID = "Nakamoto",
           int val = 1, int d = 1, int m = 1, int y = 2011,
           int h = 1, int min = 1, TreeNode * nodeLoc = NULL, int coinInv = 1)
{
  tx = new Transaction(txID, sID, rID, val, d, m, y, h, min, coinInv);
  treePos = nodeLoc;
  //cout << "Node constructed successfully!\n";
}

Node::~Node()
{
  delete tx;
  //cout << "Node destroyed succesfully!\n";
}

bool Node::traceCoinHistory(const int & target, int * txIDs, const int & sz) const
{
  bool result = tx->traceCoinHistory(target, txIDs, sz);
  return result;
}

int Node::searchBTCTxsInv(const int & BTCId, int * txIDs, const int & sz) const
{
  int result = tx->searchBTCTxsInv(BTCId, txIDs, sz);
  return result;
}

int Node::findEarnings(string owner, const int & sDay, 
                        const int & sMonth, const int & sYear, 
                        const int & sHour, const int & sMinute,
                        const int & eDay, const int & eMonth, 
                        const int & eYear, const int & eHour,
                        const int & eMinute, int & lastTxID) const
{
  int result = tx->findEarnings(owner, sDay, sMonth, sYear, sHour, sMinute, 
                                eDay, eMonth, eYear, eHour, eMinute, lastTxID);
  return result;
}

int Node::findPayments(string owner, const int & sDay, 
                        const int & sMonth, const int & sYear, 
                        const int & sHour, const int & sMinute,
                        const int & eDay, const int & eMonth, 
                        const int & eYear, const int & eHour,
                        const int & eMinute, int & lastTxID) const
{
  int result = tx->findPayments(owner, sDay, sMonth, sYear, sHour, sMinute, 
                                eDay, eMonth, eYear, eHour, eMinute, lastTxID);
  return result;
}                                          

TransactionsList::TransactionsList(string wID = "FluffyPony")
: head(NULL), walletID(wID), size(0)
{
  //cout << "TransactionsList constructed successfully!\n";
}

TransactionsList::~TransactionsList()
{
  Node * cur = head;
  Node * temp = NULL;
  while(cur != NULL)
  {
    temp = cur;
    cur = cur->next;
    delete temp;
  }
  //cout << walletID << " TransactionsList deleted succesfully!\n";
}

void TransactionsList::addTx(const int & txID, string sender, string receiver, 
                             const int & amount, const int & day,
                             const int & month, const int & year,
                             const int & hour, const int & min, TreeNode * invo,
                             const int & coinInv)
{
  Node * node = new Node(txID, sender, receiver, amount, day, month, year,
                         hour, min, invo, coinInv);
  node->next = head;
  head = node;
  size++;
}

void TransactionsList::setWalletID(string newID)
{
  walletID = newID;
  //cout << "claimed by " << walletID << endl;
}

bool TransactionsList::traceCoinHistory(const int & target) const
{
  bool result = false;
  int * txIDs = new int[size];//used to skip 
  for(int i = 0; i < size; i++)//init
    txIDs[i] = -1;
  Node * cur = head;
  while(cur != NULL)
  {
    if(cur->traceCoinHistory(target, txIDs, size) == true)
      result = true;
    cur = cur->next;
  }
  delete[] txIDs;
  return result;
}

int TransactionsList::searchBTCTxsInv(const int & BTCId) const
{
  int result = 0;
  int * txIDs = new int[size];//used to skip 
  for(int i = 0; i < size; i++)//init
    txIDs[i] = -1;
  Node * cur = head;
  while(cur != NULL)
  {
    result += cur->searchBTCTxsInv(BTCId, txIDs, size);
    cur = cur->next;
  }
  delete[] txIDs;
  return result;
}

string TransactionsList::getWalletID() const
{
  return walletID;
}

void TransactionsList::findEarnings(string owner, const int & sDay, 
                                    const int & sMonth, const int & sYear, 
                                    const int & sHour, const int & sMinute,
                                    const int & eDay, const int & eMonth, 
                                    const int & eYear, const int & eHour,
                                    const int & eMinute) const
{
  Node * cur = head;
  int lastTxID = -1;
  int sum = 0;
  while(cur != NULL)
  {
    sum += cur->findEarnings(owner, sDay, sMonth, sYear, sHour, sMinute, 
                                eDay, eMonth, eYear, eHour, eMinute, lastTxID);
    cur = cur->next;
  }
  cout << endl;
  if(sum > 0)
  {
    cout << owner << "'s total earnings from above transactions : ";
    cout << sum << endl;
  }
  else
    cout << owner << " has not received any BTCs yet!\n";
}

void TransactionsList::findPayments(string owner, const int & sDay, 
                                    const int & sMonth, const int & sYear, 
                                    const int & sHour, const int & sMinute,
                                    const int & eDay, const int & eMonth, 
                                    const int & eYear, const int & eHour,
                                    const int & eMinute) const
{
  Node * cur = head;
  int lastTxID = -1;
  int sum = 0;
  while(cur != NULL)
  {
    sum += cur->findPayments(owner, sDay, sMonth, sYear, sHour, sMinute, 
                                eDay, eMonth, eYear, eHour, eMinute, lastTxID);
    cur = cur->next;
  }
  cout << endl;
  if(sum > 0)
  {
    cout << owner << "'s total payments from above transactions : ";
    cout << sum << endl;
  }
  else
    cout << owner << " has not sent any BTCs yet!\n";
}                                             
#ifndef __WALLET__
#define __WALLET__

#include <string>
using std::string;

class CoinNode
{
private:
  int coinID;//BTC unique ID
  int value;//BTC fragment's value

public:
  CoinNode * next;//points to the next node
  CoinNode(int, int);//constructor
  ~CoinNode();//destructor
  int getVal() const;//get coin value
  int getID() const;//get coin id
  void setVal(const int &);//set coin value
};

class HODLStack
{
private:
  CoinNode * head;//head node of the stack
  int size;//total nodes linked

public:  
  HODLStack();//constructor
  ~HODLStack();//destructor
  void addNode(const int &, const int &);//adds a new BTC node
  void delNode();//deletes emptied BTC nodes
  void getBTCs(int *, int *, const int &, int &);//get data for txs
};

class Wallet
{
public:
  Wallet(int, int, int *, string);//constructor
  ~Wallet();//destructor
  int getSize() const;//get number of distinct BTCs
  int getBalance() const;//get wallet's total balance
  void setBalance(const int &);//update wallet's balance after tx
  void setSize(const int &);//update wallet's size after removing or adding BTCs
  string getID() const;//get wallet owner's ID
  int checkBalance(string, const int &) const;//check if tx can proceed
  //and which coins can participate in it
  int checkRcvr(string) const;//checks if owner exists
  void getBTCs(int *, int *, const int &, int &);//get data for txs from HODLStk
  void addBTCs(int * , int *, const int &, const int &);//gets IDs and updates
  void delBTCs();//deletes emptied BTC nodes

private:
  int totalBalance;//sum of holdings
  int size;//total distinct bitcoins
  HODLStack * ledger;//holds bitcoin ids and values
  string walletID;//wallet owner ID
};

#endif
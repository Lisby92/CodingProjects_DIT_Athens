#ifndef __TRANSACTIONSLIST__
#define __TRANSACTIONSLIST__
#include "TimeFrame.hpp"
#include <string>
#include "BlockchainTree.hpp"
using std::string;

class Transaction
{
public:
  Transaction(int, string, string, int, int, int, int,
              int, int, int);//constructor
  ~Transaction();//destructor
  bool traceCoinHistory(const int &, int *, const int &) const;
  //trace a BTC's transaction history
  int getValue() const;//get transaction value
  int findEarnings(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &, int &) const;
  //find earnings of a wallet user based on time range or totally
  int findPayments(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &, int &) const;
  //find payments of a wallet user based on time range or totally
  int searchBTCTxsInv(const int &, int *, const int &) const;
  //search number of transactions a BTC has participated

private:
  int transactionID;//unique transaction id > 0
  string senderWalletID;//unique sender wallet ID
  string receiverWalletID;//unique receiver Wallet ID
  int value;//amount transfered
  int coinInvolved;//BTC Id involved in the transaction
  TimeFrame * timeFrame;//date and time of transaction
};

class Node
{
public:
  Node * next;//pointer to the next node
  Node(int, string, string, int, int, int, int, int, int,
       TreeNode *, int);//constructor
  ~Node();//destructor
  bool traceCoinHistory(const int &, int *, const int &) const;
  //trace a BTC's transaction history
  int findEarnings(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &, int &) const;
  //find earnings of a wallet user based on time range or totally
  int findPayments(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &, int &) const;
  //find payments of a wallet user based on time range or totally
  int searchBTCTxsInv(const int &, int *, const int &) const;
  //search number of transactions a BTC has participated

private:
  Transaction * tx;//data of each node = 1 transaction
  TreeNode * treePos;//pointer to the coins tree node
};

class TransactionsList
{
public:
  TransactionsList(string);//constructor
  ~TransactionsList();//destructor
  void addTx(const int &, string, string, const int &, const int &, const int &,
             const int &, const int &, const int &, TreeNode *, const int &);
  //add and link a new transaction node
  void setWalletID(string);//set list's wallet user ID
  bool traceCoinHistory(const int &) const;
  //trace a BTC's transaction history
  string getWalletID() const;//get list's wallet user ID
  void findEarnings(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &) const;
  //find earnings of a wallet user based on time range or totally
  void findPayments(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &) const;
  //find payments of a wallet user based on time range or totally
  int searchBTCTxsInv(const int &) const;
  //search number of transactions a BTC has participated

private:
  Node * head;//the first node in the list
  string walletID;//list's wallet id
  int size;//number of current transactions
};

#endif
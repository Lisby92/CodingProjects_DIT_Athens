#ifndef __BLOCKCHAINTREE__
#define __BLOCKCHAINTREE__
#include <string>

using std::string;

class TreeNode
{
public:
  TreeNode(string, int);//constructor
  ~TreeNode();//destructor
  string getID() const;//get wallet id
  int getVal() const;//get current value
  bool isEmpty() const;//check if tree is empty
  int addNode(string, string, const int &, const int &, TreeNode *);
  void searchBTCTxsInv(int &) const;
  //finds total transactions much faster than hash table
  //but counts transactions more than once if the coin has 
  //already been fragmented from its initial value from previous txs
  void searchBTCUnspent(int &, string) const;
  //search for unspent value of BTCs based on initial owners
  TreeNode * left;//left child
  TreeNode * right;//right child
  
private:
  string walletID;//initial owner for root, curOwner for sublevels
  int value;//initial value for root
  int bitcoinID;//the bitcoin's unique identifier
};

class BlockchainTree
{
public:
  BlockchainTree(string, int, int);//constructor
  ~BlockchainTree();//destructor
  int getCoinID() const;//get coin id
  TreeNode * root;//the root node

private:
  int bitcoinID;//the bitcoin's unique identifier
};

#endif
#include "BlockchainTree.hpp"
#include <iostream>

using namespace std;

TreeNode::TreeNode(string owner, int val)
: left(NULL), right(NULL), walletID(owner), value(val)
{
  //cout << "Owner : " << walletID << ", ";
  //cout << "Bitcoin ID : " << bitcoinID << ", ";
  //cout << "Value : " << value << endl;
  //cout << "TreeNode constructed successfully!\n";
}

TreeNode::~TreeNode()
{
  //cout << "Owner : " << walletID << ", ";
  //cout << "Bitcoin ID : " << bitcoinID << ", ";
  //cout << "Value : " << value << endl;
  if(left != NULL && right != NULL)
  {
    //cout << "Deleting left child\n";
    delete left;
    //cout << "Deleting right child\n";
    delete right;
  }
  //cout << "TreeNode destoyed successfully!\n";
}

string TreeNode::getID() const
{
  return walletID;
}

int TreeNode::getVal() const
{
  return value;
}

bool TreeNode::isEmpty() const
{
  return (this == NULL);
}

int TreeNode::addNode(string sender, string receiver, 
                      const int & val, const int & id, TreeNode * involvedNode)
{
  //cout << "entering node owner " << walletID << endl;
  //cout << "with bitcoin id " << bitcoinID << endl;
  //cout << "and value " << value << endl;
  if(sender.compare(walletID) == 0)//sender node found
  {
    if(left == NULL && right == NULL)//leaf
    {
      if(val > value)
        return 0;
      //cout << "creating left and right\n";
      left = new TreeNode(receiver, val);
      involvedNode = left;
      right = new TreeNode(sender, (value-val));
      return 1;//added 
    }
    else//branch
    {
      //cout << "going right\n";
      right->addNode(sender, receiver, val, id, involvedNode);
    }
  }
  else//go deeper
  {
    if(left != NULL)
    {
      //cout << "going left\n";
      if(left->addNode(sender, receiver, val, id, involvedNode) == 1)
        return 1;//all ok from left
      else
      {
        if(right != NULL)
        {
          //cout << "going right\n";
          if(right->addNode(sender, receiver, val, id, involvedNode) == 1)
            return 1;//all ok from right
          else
            return 0;//weird error not supposed to get here
        }
      }
    }
  }
  if(left != NULL)
    {
      //cout << "going left\n";
      if(left->addNode(sender, receiver, val, id, involvedNode) == 1)
        return 1;//all ok from left
      else
      {
        if(right != NULL)
        {
          //cout << "going right\n";
          if(right->addNode(sender, receiver, val, id, involvedNode) == 1)
            return 1;//all ok from right
          else
            return 0;//weird error not supposed to get here
        }
      }
    }
    return 0;
}

void TreeNode::searchBTCTxsInv(int & txsInvolved) const
{
  if(left == NULL && right == NULL)//that was the last transaction
    return;
  else//recurse all children
  {
    txsInvolved++;//found
    //cout << "In TreeNode " << walletID;
    //cout << ", with value : " << value << endl;
    //cout << "Going left \n";
    left->searchBTCTxsInv(txsInvolved);
    //cout << "Going right \n";
    right->searchBTCTxsInv(txsInvolved);
  }
}
void TreeNode::searchBTCUnspent(int & unitsUnspent, string owner) const
{
  //cout << "in node : " << walletID << ", with val : " << value << endl;
  if(owner.compare(getID()) == 0)
    unitsUnspent = getVal();
  if(left == NULL && right == NULL)//that was the last transaction
    return;
  else//recurse right children = remaining of initial owner
  {
    //cout << "going right\n";
    right->searchBTCUnspent(unitsUnspent, owner);
  }
}

BlockchainTree::BlockchainTree(string owner = "Satoshi",
                               int val = 1000, int id = 1)
: bitcoinID(id)
{
  root = new TreeNode(owner, val);
  //cout << "BlockchainTree constructed successfully!\n";
}

BlockchainTree::~BlockchainTree()
{
  if(root != NULL)
    delete root;
  //cout << bitcoinID << " BTC's tree destructed successfully!\n";
}

int BlockchainTree::getCoinID() const
{
  return bitcoinID;
}
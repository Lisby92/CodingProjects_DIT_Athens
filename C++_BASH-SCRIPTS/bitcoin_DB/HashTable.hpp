#ifndef __HASHTABLE__
#define __HASHTABLE__
#include <string>
#include "TransactionsList.hpp"
using std::string;

class Bucket
{
public:
  Bucket(int);//constructor
  ~Bucket();//destructor
  Bucket * next;//pointer to next bucket of same hash result (used if no space)
  void addTx(string, string, const int &, const int &, const int &,
              const int &, const int &, const int &, const int &,
              TreeNode *, const int &, const int &, const int &);
  //adds a new valid transaction at the corresponding bucket
  int searchID(string);//search for a wallet user
  int getCap() const;//get total users added in the bucket
  int getSize() const;//get bucket's max size
  void addWalletID(string, const int &);//add a new wallet ID
  bool traceCoinHistory(const int &) const;//trace a BTC's history
  void findEarnings(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &);
  //find earnings of a wallet user based on time range or totally
  void findPayments(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &);
  //find payments of a wallet user based on time range or totally
  int searchBTCTxsInv(const int &) const;
  //search number of transactions a BTC has participated

private:
  int size;//size of each bucket in bytes provided from user (command line)
  int curCap;//current bucket capacity
  TransactionsList ** txArray;//array of pointers to transaction list heads
  string * walletArray; //array of wallet users in the bucket
};

class BucketList
{
public:
  BucketList(int, int);//constructor
  ~BucketList();//destructor
  void addBucket(const int &);//add and link a new bucket, after filling
  int getID() const;//get List's ID
  void addTx(string, string, const int &, const int &, const int &,
              const int &, const int &, const int &, const int &,
              TreeNode *, const int &, const int &);
  //adds a new valid transaction at the corresponding bucket
  int searchID(string);//search for a wallet user
  int getBucketSize() const;//get bucket size
  bool traceCoinHistory(const int &) const;
  void findEarnings(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &);
  //find earnings of a wallet user based on time range or totally
  void findPayments(string , const int &, const int &, const int &,
                   const int &, const int &, const int &, const int &,
                   const int &, const int &, const int &);
  //find payments of a wallet user based on time range or totally
  int searchBTCTxsInv(const int &) const;
  //search number of transactions a BTC has participated
private:
  int size;//total buckets linked
  int id;//unique list id
  int bucketSize;//size of each bucket
  Bucket * head;//first node of the list
};

class HashTable
{
public:
  HashTable(int, int);//constructor
  ~HashTable();//destructor
  int hashFunction(string);//returns an integer hash from a given string
  void findAdd(string, string, const int &, const int &, const int &,
              const int &, const int &, const int &,
              const int &, const int &, TreeNode *, const int &);
  //finds the correct bucket then 
  //adds a new valid transaction at the corresponding bucket
  bool traceCoinHistory(const int &) const;//trace BTCs history
  bool findEarnings(string , string &, string &, string &, string &);
  //find earnings of a wallet user based on time range or totally
  bool findPayments(string , string &, string &, string &, string &);
  //find payments of a wallet user based on time range or totally
  int searchBTCTxsInv(const int &) const;
  //search number of transactions a BTC has participated

private:
  int size;//number of max distinct hash table bucket heads
  BucketList ** bucketArray;//array of pointers to bucket lists
};

#endif
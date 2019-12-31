#ifndef __HELPERS__
#define __HELPERS__
#include <string>
#include <ctime>
using std::string;
//helper classes to detect false input at balances
class BTCNode
{
public:
  BTCNode(int, string);//constructor
  ~BTCNode();//destructor
  BTCNode * next;//points to next node
  void print() const;//print data
  int getID() const;//get BTC Id
  string getOwner() const;//get owner ID
private:
  int BTCid;//coin's unique identifier
  string owner;//owner's unique identifier
};

class List
{
public:
  List();//constructor
  ~List();//destructor
  void addNode(const int &, string);//adds and links a new node
  void print() const;//print all nodes data
  bool searchDuplicate() const;//search for duplicate BTC Ids
  int getSize() const;//get list's size
  void getIDs(int *) const;//get all the bitcoin ids
  void getOwners(string *) const;//get all the owner ids
  bool searchID(const int &) const;//search for specific coin ID

private:
  BTCNode * head;//first node of the list
  int size;//total nodes
};

bool isNumber(string);//checks if given string is a number
bool dateCheck(const int &, const int &, const int &, const int &, const int &,
               const int &, const int &, const int &, const int &, const int &);
//checks if dates given are concurrent or wrong
bool breakDownTx(string &, int &, string &, string &, int &,
                 int &, int &, int &, int &, int &);
//breaks a transaction string down to components for processing  
//uses current PC time if date and time is missing and checks 
//if transaction data are valid              
#endif
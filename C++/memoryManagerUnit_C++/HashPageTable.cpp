/*************************************************************************
Implementation File : HashPageTable.cpp
Author Date     : V. Mavrommatis 27/12/2018
Purpose       : Implementation of a hash PT with Seperate Chaining aka 
                open hashing for conflict resolution and temporal locality
Revision      : < Vasileios_Mavrommatis - 2/1/2019 >
**************************************************************************/
#include <string>
#include <iostream>
#include <cstdlib>
#include "HashPageTable.h"
using namespace std;

Node::Node(int p = 0, int f = 0, int proc = 0, bool d = false, bool v = true)
{
  pte = new PageTableEntry(p, f, proc, d, v);
  //cout << "Node created succesfully!\n";
}

Node::~Node()
{
  delete pte;
  //cout << "Node destroyed succesfully!\n";
}

int Node::getVPN() const
{
  return pte->getPage();
}

int Node::getPFN() const
{
  return pte->getFrame();
}

int Node::getSrc() const
{
  return pte->getPid();
}

void Node::setD(const bool & bit)
{
  pte->setDirtyBit(bit);
}

bool Node::getD() const
{
  return pte->getDirtyBit();
}

void Node::setV(const bool & bit)
{
  pte->setValidBit(bit);
}

bool Node::getV() const
{
  return pte->getValidBit();
}

Chain::Chain()
: head(NULL), id(0)
{
  //cout << "Chain created succesfully!\n";
}

Chain::~Chain()
{
  Node * cur = head;
  Node * temp = NULL;
  while(cur != NULL)
  {
    temp = cur;
    cur = cur->next;
    delete temp;
  }
  //cout << "Chain deleted succesfully!\n";
}

void Chain::setId(const int & number)
{
  id = number;
}

void Chain::addNode(const int & p, const int & f, const int & proc, const bool & d, const bool & v)
{
  Node * node = new Node(p, f, proc, d, v);
  node->next = head;
  head = node;
}

int Chain::searchNode(const int & target, int & searches, const bool & bit, const int & proc, int & replacedFrame)
{
  Node * cur = head;
  Node * spot = NULL;
  Node * fix = NULL;
  int flag = 0;//signals cases
  //cout << "SEARCHING!\n";
  while(cur)
  {
    if(cur->getSrc() == proc)//dont mess with other process entries that can be in same chain
    { 
      //cout << "checking node with PFN " << cur->getPFN() << " and VPN " << cur->getVPN() << " and valid bit " << cur->getV() << " and dirty bit " << cur->getD() << endl;
      if(cur->getVPN() == target)//found my target //general case 1 found entry 
      {
        if(cur->getV() == true)//valid page
        {
          if(bit == true)//set dirty bit if write
            cur->setD(bit);
          return cur->getPFN();//return valid frame number //the best case
        }
        else if(cur->getV() == false)//was kicked out by another page or FWF
        {
          fix = cur;
          flag = 1;
          cur->setV(true);//valid again
          if(bit == true)//set dirty bit if write
            cur->setD(bit);
          break;
        }
      }
      else//not my target//general case 2
      {
        if(cur->getV() == true)//but is valid
        {
          cur->setV(false);//invalidate on path
          if(cur->getD() == true)//dirty frame
          { 
            spot = cur;
            flag = 1;
            replacedFrame = cur->getPFN();
          }
        }
      }
    }
    cur = cur->next;
    searches++;
  }
  //end of searching
  if(cur == NULL && flag == 1)
  {
    //cout << "Adding at Chain: " << id << " VPN: " << target << " PFN: " << replacedFrame << " Dirty_bit: " << bit << " Valid_bit: 1\n";
    //replacedFrame = spot->getPFN();
    spot->setD(false);
    spot->setV(false);
    addNode(target, replacedFrame, proc, bit, true);
    return -3;
  }
  if(flag == 0)//couldnt found entry
    return -2;
  else if(flag == 1)//found entry and is invalid and frame is clean
  {
    cur = head;
    while(cur)
    {
      if(cur->getSrc() == proc)//dont mess with others
      {
        if(cur->getV() == true)
        {
          if(cur != fix)
            cur->setV(false);
        }
      }
      cur = cur->next;
    }
    if(replacedFrame != -1)
      spot->setD(false);
    return -1;
  }
  else//(flag == 2)
    return -3;
}

void Chain::flushNode(const int & src, int * victims, const int & size)
{
  Node * cur = head;
  //cout << "FLUSHING CHAIN " << id << "\n";
  while(cur)
  {
    if(cur->getSrc() == src)//dont mess with other processes
    {
      //cout << "checking node with PFN " << cur->getPFN() << " and VPN " << cur->getVPN() << " and valid bit " << cur->getV() << " and dirty bit " << cur->getD() << endl;
      if(cur->getV() == true)
      {
        cur->setV(false);//invalidate entry
        //cout << "Flushing frame " << cur->getPFN() << "\n";
        //cout << "with VPN " << cur->getVPN() << "\n";
        if(cur->getD() == true)//page must be written at disk
        {
          for(int i = 0; i < size; i ++)
          {
            if(victims[i] == -1)//empty slot
            {
              victims[i] = cur->getPFN();
              cur->setD(false);
              break;
            }
          }
        }
      }
      if(cur->getV() == false)
      {
        if(cur->getD() == true)//page must be written at disk
        {
          for(int i = 0; i < size; i ++)
          {
            if(victims[i] == -1)//empty slot
            {
              victims[i] = cur->getPFN();
              cur->setD(false);
              break;
            }
          }
        }
      }
    } 
    cur = cur->next;
  }
}

int Chain::findFrames() const
{
  Node * cur = head;
  int used = 0;
  while(cur)
  {
    if(cur->getV() == true)//found a used frame
    {
      used++;
      //cout << " found a used with VPN " << cur->getVPN() << " @ chain " << id << endl;
    }
    cur = cur->next;
  }
  return used;
}

PageTableEntry::PageTableEntry(int p = 0, int f = 0, int proc = 0, bool d = false, bool v = true)
:  vpn(p), pfn(f), pid(proc), dirtyBit(d), validBit(v)
{
  //cout << "PageTableEntry created!\n";
}

PageTableEntry::~PageTableEntry()
{
  //cout << "PageTableEntry deleted!\n";
}

int PageTableEntry::getPage() const
{
  return vpn;
}

int PageTableEntry::getFrame() const
{
  return pfn;
}

int PageTableEntry::getPid() const
{
  return pid;
}

void PageTableEntry::setDirtyBit(const bool & bit)
{
  dirtyBit = bit;
}

bool PageTableEntry::getDirtyBit() const
{
  return dirtyBit;
}

void PageTableEntry::setValidBit(const bool & bit)
{
  validBit = bit;
}

bool PageTableEntry::getValidBit() const
{
  return validBit;
}

HashPageTable::HashPageTable(int entries, int k)
: size(entries), maxFaults(k), curFaults1(0), curFaults2(0)
{
  pteArray = new Chain[entries];
  for(int i = 0; i < entries; i++)
    pteArray[i].setId(i);
  //cout << "HashPageTable created with " << entries << " hashed chains\n";
}

HashPageTable::~HashPageTable()
{
  delete[] pteArray;
  //cout << "HashPageTable destroyed succesfully!\n";
}

void HashPageTable::addEntry(const int & p, const int & k, const int & f, const int & proc, const bool & d, const bool & v)
{
  //cout << "Adding at Chain: " << k << " VPN: " << p << " PFN: " << f << " Dirty_bit: " << d << " Valid_bit: " << v <<"\n";
  pteArray[k].addNode(p, f, proc, d, v);
}

int HashPageTable::hashFunction(const int & key) const
{
  return (key % size);
}

int HashPageTable::find(const int & hash, const int & key, int & searches, const bool & dirty, const int & proc, int & frameR) const
{
  int result = pteArray[hash].searchNode(key, searches, dirty, proc, frameR);
  return result;
}

int HashPageTable::getMaxPF() const
{
  return maxFaults;
}

int HashPageTable::getCurPF(const int & src) const
{
  if(src == 1)//process1
    return curFaults1;
  else//process2
    return curFaults2;
}

void HashPageTable::setCurPF(const int & src, const int & faults)
{
  if(src == 1)//process1
    curFaults1 = faults;
  else//process2
    curFaults2 = faults;
}

void HashPageTable::flushEntries(const int & src, int * victims, const int & size)
{
  for(int i = 0; i < size; i++)
    pteArray[i].flushNode(src, victims, size);
}

int HashPageTable::findUsed() const
{
  int used = 0;
  for(int i = 0; i < size; i++)
    used += pteArray[i].findFrames();
  return used;
}